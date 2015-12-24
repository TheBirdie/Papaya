#pragma once
#include <vector>
#include <QImage>
#include <QFileInfo>
#include "glm/glm.hpp"

class Mesh
{
  public:
    Mesh(): m_radius(1) {}
    bool LoadObjModel(const char *filename);
    bool LoadMTLFile(const char* path, const char* filename);
    void LoadOpenGLTextures();
    void Draw();					// Draws the model on the screen
    void Release();				// Release the model
    float GetRadius() const { return m_radius; }
    glm::vec3 const& GetCenter() const { return m_center; }
    bool IsLoaded() const { return vertices.size() && faceIndex.size(); }

    // Positions
    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec2>  texturePositions;
    // Triangles
    std::vector<int>        textureIndex;
    std::vector<int>        faceIndex;
    std::vector<int>        normalIndex;
    std::vector<std::pair<int /* faceIdx */, int /* texture to use*/> > triangleTextures;
    // Textures
    struct TextureInfo
    {
        GLuint texHandle;
        glm::vec3 Ka, Kd, Ks;
        QFileInfo map_Kd;
        QImage map_Kd_gl;
        QFileInfo map_bump;
        QFileInfo bump;
    };

    std::map<std::string, int> textureNameToId;
    std::vector<TextureInfo>   textures;

    // Model general information
    float                   m_radius;
    glm::vec3               m_center;
};
