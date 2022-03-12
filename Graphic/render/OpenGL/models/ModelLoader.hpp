#pragma once

#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <string>
#include <vector>

#include "../shader/ShaderProgram.hpp"
#include "MeshLoader.hpp"

namespace Graphic
{
    class Model
    {
    public:
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;
        std::string directory;
        bool gammaCorrection;

        Model(const char* path);
        void Draw(ShaderProgram& shader);

    private:
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };

}