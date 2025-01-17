#ifndef MODEL_HPP
#define MODEL_HPP
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>
#include "shader.hpp"
#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <string>

struct Bone {
    std::string name;
    int parentId;
    glm::mat4 offset;
};

class Model
{
    public:
        Model(char *path);
        Model(const Model& x);
        Model& operator=(const Model& x);
        void draw(Shader &shader, float curTime = 0.0f);
        Mesh getMesh();
        std::vector<Mesh> getMeshes();
        bool isLoaded();
    private:
        std::vector<Mesh> meshes;
        aiAnimation* animation;
        std::vector<Bone> bones;

        std::vector<glm::mat4> localBoneTransformMatrices;
        std::vector<glm::mat4> finalBoneTransformMatrices;

        std::string directory;
        int numBones;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        aiNode* findRootBone(const aiScene *scene);
        bool isBoneOnlyNode(aiNode *node);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        void processBoneTree(aiNode *node, int parentIdx);
        void loadBones(const aiScene *scene);
        void loadAnimations(float curTime);
        int getChannelPositionIndex(aiNodeAnim *channel, float timePoint);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

        inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 from)
        {
            glm::mat4 to;


            to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
            to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
            to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
            to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

            return to;
        }
};

#endif