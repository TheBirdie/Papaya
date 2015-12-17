#pragma once
#include <vector>
#include "glm/glm.hpp"

class Mesh
{
  public:
    Mesh() {}
    bool LoadObjModel(const char *filename);
    void Draw();					// Draws the model on the screen
    void Release();				// Release the model

    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec2>  texture;
    // Triangles
    std::vector<int>        textureIndex;
    std::vector<int>        faceIndex;
};
