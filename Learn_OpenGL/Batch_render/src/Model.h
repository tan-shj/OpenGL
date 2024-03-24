#pragma once
#include "Shader.h"
#include "Mesh.h"

#include <iostream>
#include <vendor/stb_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    /*  ��Ա����   */
    Model(std::string const& path);
    void Draw(Shader shader);
private:
    /*  ģ������  */
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    /*  ˽�г�Ա����   */
    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};