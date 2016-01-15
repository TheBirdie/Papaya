#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <QDebug>

#include "vector.h"
#include "ray.h"
#include "triangle.h"

class KDNode {
public:
    KDNode(): left(NULL), right(NULL) {}
    ~KDNode()
    {
        for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); ++it)
            delete *(it);
        if (left)
            delete left;
        if (right)
            delete right;
    }

    AABBox box;
    KDNode* left;
    KDNode* right;
    std::vector<Triangle*> triangles;
    bool leaf;

    void build(std::vector<Triangle*> &tris, int depth = 0);
    bool hit(const Ray &ray, double &t, double &tmin, Vec &normal, Vec &c) const;
};

typedef KDNode KDTree;

#endif // KDTREE_H
