#pragma once
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<string>
#include<vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    GLuint id;
    std::string type;
    aiString path;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> texture);
    void Draw(Shader shader);

private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};