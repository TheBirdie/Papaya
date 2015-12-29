#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <GL/glew.h>
//#include <GL/GL.h>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QImage>
#include <QGLWidget>
#include "objloader.h"

#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9

using namespace std;

bool Mesh::LoadObjModel(const char *filename)
{
    QFileInfo path(filename);
    float minMaxCoords[6] = {0};
    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
        std::cerr << "Cannot open " << filename << std::endl;
        return false;
    }

    int lineIdx = -1;
    std::string line;
    while (std::getline(in, line))
    {
        ++lineIdx;
        if (lineIdx % 50000 == 0)
            qDebug() << "Processing line " << lineIdx << " ...";
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
        else if (line.substr(0,3)=="vn ")
        {
            std::istringstream v(line.substr(3));
            glm::vec3 vert;
            double x,y,z;
            v>>x;v>>y;v>>z;
            vert=glm::vec3(x,y,z);
            normals.push_back(vert);
        }
        //check for mtl file
        else if (line.substr(0,7)=="mtllib ")
        {
            // Trim the line
            line.erase(line.find_last_not_of(" \n\r\t")+1);
            if (!LoadMTLFile(path.canonicalPath().toStdString().c_str(), line.substr(7).c_str()))
                qWarning() << filename << ':' << lineIdx << "Unable to load MTL file";
        }
        //check for mtl file
        else if (line.substr(0,7)=="usemtl ")
        {
            std::string textureName(line.substr(7).c_str());
            int textureIdx = -1;
            if (textureNameToId.find(textureName) != textureNameToId.end())
                textureIdx = textureNameToId[textureName];

            triangleTextures.push_back(std::pair<int,int>(faceIndex.size(), textureIdx));
        }
        //check for texture co-ordinate
        else if(line.substr(0,2)=="vt")
        {
            std::istringstream v(line.substr(3));
            glm::vec2 tex;
            float U,V;
            v>>U;v>>V;
            tex=glm::vec2(U,V);
            texturePositions.push_back(tex);
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
                // @TODO: Read non-triangle faces (generic format is "f f1 f2 f3 [f4 ...]" for a polygon)
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

void LoadVec3(const char* prefix, std::string const& line, glm::vec3& vec)
{
    if (line.substr(0,strlen(prefix)) == prefix)
    {
        std::istringstream v(line.substr(strlen(prefix)));
        v >> vec.x >> vec.y >> vec.z;
    }
}

bool LoadFilePath(const char* path, const char* prefix, std::string const& line, QFileInfo& fileInfo)
{
    if (line.substr(0,strlen(prefix)) == prefix)
    {
        std::string stdcfilename = line.substr(strlen(prefix));
        // Trim the filename
        stdcfilename.erase(stdcfilename.find_last_not_of(" \n\r\t")+1);
        fileInfo.setFile(QDir(path), QString::fromStdString(stdcfilename));
        return true;
    }
    return false;
}

bool Mesh::LoadMTLFile(const char* path, const char* filename)
{
    std::string fullpath = std::string(path) + '/' + filename;
    qDebug() << "... loading" << fullpath.c_str() << "as MTL file";
    std::ifstream in(fullpath.c_str(), std::ios::in);
    if (!in)
        return false;

    std::string line;
    int textureIndex = -1;
    while (std::getline(in, line))
    {
        if (line.substr(0,7)=="newmtl ")
        {
            textureNameToId[line.substr(7)] = ++textureIndex;
            textures.push_back(TextureInfo());
        }
        else if (textureIndex < 0)
            continue;
        LoadVec3("Kd ", line, textures[textureIndex].Kd);
        LoadVec3("Ka ", line, textures[textureIndex].Ka);
        LoadVec3("Ks ", line, textures[textureIndex].Ks);
        LoadFilePath(path, "map_Kd ", line, textures[textureIndex].map_Kd);
        LoadFilePath(path, "map_bump ", line, textures[textureIndex].map_bump);
        LoadFilePath(path, "bump ", line, textures[textureIndex].bump);
    }
    qDebug() << (textureIndex + 1) << " textures loaded.";
    return true;
}

void Mesh::LoadOpenGLTextures()
{
    for (std::vector<TextureInfo>::iterator it = textures.begin(); it != textures.end(); ++it)
    {
        QImage img(it->map_Kd.filePath());
        if (img.isNull())
        {
            qWarning() << "Unable to load texture file" << it->map_Kd.filePath();
            continue;
        }
        it->map_Kd_gl = QGLWidget::convertToGLFormat(img);  // flipped 32bit RGBA

        // Bind the img texture...
        glGenTextures(1, &it->texHandle);
        glBindTexture(GL_TEXTURE_2D, it->texHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, it->map_Kd_gl.width(), it->map_Kd_gl.height(), 0,
            GL_RGBA, GL_UNSIGNED_BYTE, it->map_Kd_gl.bits());
        qDebug() << "Texture file " << it->map_Kd.filePath() << " loaded into OpenGL texture #" << it->texHandle;
    }
}

void Mesh::Draw()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[0].texHandle);
    glBegin(GL_TRIANGLES);
    int currentTexture = -1;
    std::vector<std::pair<int, int> >::iterator textureArrayIter = triangleTextures.begin();
    assert(faceIndex.size() % 3 == 0);
    for (int i = 0; i < faceIndex.size(); ++i)
    {
        // Texture ?
        if (textureArrayIter != triangleTextures.end() && textureArrayIter->first <= i)
        {
            currentTexture = textureArrayIter->second;
            ++textureArrayIter;
            if (currentTexture >= 0)
            {
                glBindTexture(GL_TEXTURE_2D, textures[currentTexture].texHandle);
            }
        }
        // Normal ?
        if (normalIndex.size() == faceIndex.size())
        {
            int ni = normalIndex[i];
            if (ni < normals.size())
                glNormal3f(normals[ni].x, normals[ni].y, normals[ni].z);
        }
        int ti = textureIndex[i];
        glTexCoord2f(texturePositions[ti].x, texturePositions[ti].y);
        int fi = faceIndex[i];
        assert(fi < vertices.size());
        glVertex3f(vertices[fi].x, vertices[fi].y, vertices[fi].z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
