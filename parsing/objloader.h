#pragma once
#include <vector>
#include "glm/glm.hpp"

class Mesh
{
  public:
    Mesh(): m_radius(1) {}
    bool LoadObjModel(const char *filename);
    void Draw();					// Draws the model on the screen
    void Release();				// Release the model
    float GetRadius() const { return m_radius; }
    glm::vec3 const& GetCenter() const { return m_center; }

    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec2>  texture;
    // Triangles
    std::vector<int>        textureIndex;
    std::vector<int>        faceIndex;
    std::vector<int>        normalIndex;
    float                   m_radius;
    glm::vec3               m_center;
};
