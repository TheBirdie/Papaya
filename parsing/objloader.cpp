#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <QDebug>

#include "objloader.h"

#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9

using namespace std;

bool Mesh::LoadObjModel(const char *filename)
{
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
            double x,y,z;
            v>>x;v>>y;v>>z;
            vert=glm::vec3(x,y,z);
            vertices.push_back(vert);
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
            sscanf (chh, "f %i/%i/%i %i/%i/%i %i/%i/%i",
                    &vertices[0], &textures[0], &normals[0],
                    &vertices[1], &textures[1], &normals[1],
                    &vertices[2], &textures[2], &normals[2]); //here it read the line start with f and store the corresponding values in the variables


            //std::cout<<a<<b<<c<<A<<B<<C;
            for (int i = 0; i < 3; ++i)
            {
                faceIndex.push_back(vertices[i] - 1);
                textureIndex.push_back(textures[i] - 1);
            }
        }
    }
    return true;
}

void Mesh::Draw()
{
    if (true) // Full triangles
    {
        glBegin(GL_LINE_STRIP);
        assert(faceIndex.size() % 3 == 0);
        for (int i = 0; i < faceIndex.size(); ++i)
        {
            int fi = faceIndex[i];
            //qDebug() << "Vertice #" << fi << "/" << vertices.size();
            assert(fi < vertices.size());
            glVertex3f(vertices[fi].x, vertices[fi].y, vertices[fi].z);
        }
        glEnd();
    }
    // TODO(?): Other representations: points mesh ...
}