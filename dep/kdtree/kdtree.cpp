#include <vector>
#include <QDebug>

#include "vector.h"
#include "ray.h"
#include "triangle.h"
#include "kdtree.h"

// Build KD tree for tris
void KDNode::build(std::vector<Triangle*> &tris, int depth){
    leaf = false;
    triangles = std::vector<Triangle*>();
    left = NULL;
    right = NULL;
    box = AABBox();

    if (tris.size() == 0) return;

    if (depth > 25 || tris.size() <= 6) {
        triangles = tris;
        leaf = true;
        box = tris[0]->get_bounding_box();

        for (long i=1; i<tris.size(); i++) {
            box.expand(tris[i]->get_bounding_box());
        }

        left = new KDNode();
        right = new KDNode();
        left->triangles = std::vector<Triangle*>();
        right->triangles = std::vector<Triangle*>();

        return;
    }

    box = tris[0]->get_bounding_box();
    Vec midpt = Vec();
    double tris_recp = 1.0/tris.size();

    for (long i=1; i<tris.size(); i++) {
        box.expand(tris[i]->get_bounding_box());
        midpt = midpt + (tris[i]->get_midpoint() * tris_recp);
    }

    std::vector<Triangle*> left_tris;
    std::vector<Triangle*> right_tris;
    int axis = box.get_longest_axis();

    for (long i=0; i<tris.size(); i++) {
        switch (axis) {
            case 0:
                midpt.x >= tris[i]->get_midpoint().x ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
            case 1:
                midpt.y >= tris[i]->get_midpoint().y ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
            case 2:
                midpt.z >= tris[i]->get_midpoint().z ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
        }
    }

    if (tris.size() == left_tris.size() || tris.size() == right_tris.size()) {
        triangles = tris;
        leaf = true;
        box = tris[0]->get_bounding_box();

        for (long i=1; i<tris.size(); i++) {
            box.expand(tris[i]->get_bounding_box());
        }

        left = new KDNode();
        right = new KDNode();
        left->triangles = std::vector<Triangle*>();
        right->triangles = std::vector<Triangle*>();

        return;
    }

    left = new KDNode();
    right = new KDNode();
    left->build(left_tris, depth+1);
    right->build(right_tris, depth+1);
    return;
}

// Finds nearest triangle in kd tree that intersects with ray.
bool KDNode::hit(const Ray &ray, double &t, double &tmin, Vec &normal, Vec &c) const {
    double dist;
    if (box.intersection(ray, dist)){
        if (dist > tmin) return false;
        if (!leaf)
            return left->hit(ray, t, tmin, normal, c) || right->hit(ray, t, tmin, normal, c);
        else {
            bool hit_tri = false;
            long triangles_size = triangles.size();
            for (long i=0; i<triangles_size; i++) {
                if (triangles[i]->intersect(ray, t, tmin, normal)){
                    hit_tri = true;
                    tmin = t;
                }
            }
            if (hit_tri) {
                c = ray.origin + ray.direction * tmin;
                return true;
            }
        }
    }
    return false;
}
