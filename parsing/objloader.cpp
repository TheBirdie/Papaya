#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <GL/glew.h>
//#include <GL/GL.h>
#include <QDebug>

#include "objloader.h"

#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9

using namespace std;

bool Mesh::LoadObjModel(const char *filename)
{
    float minMaxCoords[6] = {0};
    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
        std::cerr << "Cannot open " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(in, line))
    {
        //check v for vertices
        if (line.substr(0,2)=="v ")
        {
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            double coords[3] = {0};
            for (int i = 0; i < 3; ++i)
            {
                v >> coords[i];
                minMaxCoords[2*i]   = fmin(minMaxCoords[2*i], coords[i]);
                minMaxCoords[2*i+1] = fmax(minMaxCoords[2*i+1], coords[i]);
            }
            vert=glm::vec3(coords[0], coords[1], coords[2]);
            vertices.push_back(vert);
        }
        //check vn for normals
        if (line.substr(0,3)=="vn ")
        {
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            double x,y,z;
            v>>x;v>>y;v>>z;
            vert=glm::vec3(x,y,z);
            normals.push_back(vert);
        }
        //check for texture co-ordinate
        else if(line.substr(0,2)=="vt")
        {
            std::istringstream v(line.substr(3));
            glm::vec2 tex;
            int U,V;
            v>>U;v>>V;
            tex=glm::vec2(U,V);
            texture.push_back(tex);
        }
        //check for faces
        else if(line.substr(0,2)=="f ")
        {
            int vertices[3] = {0};
            int textures[3] = {0};
            int normals[3]  = {0};
            const char* chh=line.c_str();
            if (sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i",
                    &vertices[0], &textures[0], &normals[0],
                    &vertices[1], &textures[1], &normals[1],
                    &vertices[2], &textures[2], &normals[2]) != 3*3 &&
                sscanf(chh, "f %i/%i %i/%i %i/%i",
                    &vertices[0], &textures[0],
                    &vertices[1], &textures[1],
                    &vertices[2], &textures[2]) != 3*2 &&
                sscanf(chh, "f %i//%i %i//%i %i//%i",
                    &vertices[0], &normals[0],
                    &vertices[1], &normals[1],
                    &vertices[2], &normals[2]) != 3*2 &&
                sscanf(chh, "f %i %i %i",
                    &vertices[0],
                    &vertices[1],
                    &vertices[2]) != 3)
            {
                qCritical() << "Wrong obj file format on the following line: " << QString::fromStdString(line);
                return false;
            }


            //std::cout<<a<<b<<c<<A<<B<<C;
            for (int i = 0; i < 3; ++i)
            {
                if (vertices[0])
                    faceIndex.push_back(vertices[i] - 1);
                if (textures[0])
                    textureIndex.push_back(textures[i] - 1);
                if (normals[0])
                    normalIndex.push_back(normals[i] - 1);
            }
        }
    }
    // Compute model radius
    for (int i = 0; i < 3; ++i)
    {
        float currentRadius = minMaxCoords[2*i+1] - minMaxCoords[2*i];
        if (currentRadius > m_radius)
            m_radius = currentRadius;
        m_center[i] = (minMaxCoords[2*i+1] + minMaxCoords[2*i]) / 2;
    }
    return true;
}

void Mesh::Draw()
{
    if (true) // Full triangles
    {
        glBegin(GL_TRIANGLES);
        assert(faceIndex.size() % 3 == 0);
        for (int i = 0; i < faceIndex.size(); ++i)
        {
            if (normalIndex.size() == faceIndex.size())
            {
                int ni = normalIndex[i];
                if (ni < normals.size())
                    glNormal3f(normals[ni].x, normals[ni].y, normals[ni].z);
            }
            int fi = faceIndex[i];
            assert(fi < vertices.size());
            glVertex3f(vertices[fi].x, vertices[fi].y, vertices[fi].z);
        }
        glEnd();
    }
    // TODO(?): Other representations: points mesh ...
}
