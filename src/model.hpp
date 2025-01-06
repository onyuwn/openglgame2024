#ifndef MODEL_HPP
#define MODEL_HPP
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.hpp"
#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class Model
{
    public:
        Model(char *path);
        Model(const Model& x);
        Model& operator=(const Model& x);
        void draw(Shader &shader);
        Mesh getMesh();
        std::vector<Mesh> getMeshes();
        bool isLoaded();
    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif