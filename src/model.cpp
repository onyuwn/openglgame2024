#include "model.hpp"

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

Model::Model(char *path) : numBones(0) {
    std::cout << "loading..." << path << std::endl;
    loadModel(path);
    std::cout << "BONES CT: " << numBones << std::endl;
    for(int i = 0; i < this->bones.size(); i++) {
        std::cout << "BONE: " << i << " " << bones[i].name << " " << bones[i].parentId << std::endl;
    }
}

Model::Model(const Model& x) : meshes(x.meshes) {
    std::cout << "copy" << x.meshes.size() << std::endl;
}

void Model::draw(Shader &shader, float curTime) {
    if(numBones > 0) {
        loadAnimations(curTime);
    }

    for(unsigned int i = 0; i < meshes.size(); i++) {
        // if(numBones > 0) {
        //     shader.setMat4Array("bone_matrices", finalBoneTransformMatrices);
        // }
        meshes[i].draw(shader);
    }
}

std::vector<Mesh> meshes;
std::string directory;

void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    std::cout << "LOADING BONES" << std::endl;
    loadBones(scene);
    processNode(scene->mRootNode, scene);

    if(scene->mNumAnimations > 1) {
        std::cout << "LOADING ANIMATION" << std::endl;
        aiAnimation *animation = scene->mAnimations[1];
        std::cout << animation->mName.C_Str() << std::endl;
        std::cout << animation->mDuration << std::endl;
        this->animation = scene->mAnimations[1];
    }
}

void Model::loadAnimations(float curTime) { // i thinks we call this each frame? or load once, pass matrices to vert shader, iterated there? idk what to do about curTime lol
    aiAnimation *animation = this->animation; // okay so exporting from blender -- don't check NLA strips but all actions. bake animations from pose mode
    
    if(!animation) {
        std::cout << "failed to load animation " << std::endl;
        return; 
    }

    std::cout << "ANIM " << animation->mName.C_Str() << std::endl;
    float durationInSec = animation->mTicksPerSecond * animation->mDuration;
    float animationTimePoint = std::fmod(curTime, durationInSec);

    for(int i = 0; i < numBones; i++) {
        std::cout << "PROCESSING BONE " << i << std::endl;
        Bone bone = bones[i];
        std::cout << "BONE PARENT: " << bone.parentId << std::endl;
        glm::mat4 parent_transform = glm::mat4(1.0);

        if(i > 0 && localBoneTransformMatrices.size() > 0) {
            parent_transform = localBoneTransformMatrices[bone.parentId];
        } else {
            std::cout << "no parent or no bone transformation matrices..." << std::endl;
        }

        if(i < animation->mNumChannels - 1) {
            std::cout << "CHANNELS" << animation->mNumChannels << " " << animationTimePoint << std::endl;
            aiNodeAnim *channel = animation->mChannels[i];
            if(!channel || channel == nullptr) {
                std::cout << "invalid channel..." << std::endl;
                return;
            }
            int idxPosition = getChannelPositionIndex(channel, animationTimePoint);
            std::cout << "FOUND CHANNEL POSSITION INDEX " << idxPosition << std::endl;
            std::cout << "position keys: " << channel->mNumPositionKeys << std::endl;
            std::cout << "rotation keys: " << channel->mNumRotationKeys << std::endl;
            std::cout << "scale keys: " << channel->mNumScalingKeys << std::endl;
            if(channel->mNumPositionKeys > 0) {
                glm::mat4 translation = glm::translate(
                glm::mat4(1.0),
                glm::vec3(channel->mPositionKeys[idxPosition].mValue.x,
                        channel->mPositionKeys[idxPosition].mValue.y,
                        channel->mPositionKeys[idxPosition].mValue.z)
                );

                glm::mat4 rotation = glm::toMat4(
                    glm::normalize(
                        glm::quat(channel->mRotationKeys[idxPosition].mValue.x,
                                channel->mRotationKeys[idxPosition].mValue.y,
                                channel->mRotationKeys[idxPosition].mValue.z,
                                channel->mRotationKeys[idxPosition].mValue.w)
                    )
                );

                glm::mat4 scale = glm::scale(
                    glm::mat4(1.0),
                    glm::vec3(channel->mScalingKeys[idxPosition].mValue.x,
                            channel->mScalingKeys[idxPosition].mValue.y,
                            channel->mScalingKeys[idxPosition].mValue.z)
                );


                glm::mat4 animTransform = translation * rotation * scale;
                //glm::mat4 animTransform = glm::mat4(1.0);
                glm::mat4 localTransform = parent_transform * animTransform;
                localBoneTransformMatrices.push_back(localTransform);
                glm::mat4 finalTransform = localTransform * bone.offset;
                finalBoneTransformMatrices.push_back(finalTransform);
            } else {
                std::cout << "NO POSITION KEYS DAFUQ" << std::endl;
            }
        }
    }
}

int Model::getChannelPositionIndex(aiNodeAnim *channel, float timePoint) { // todo: use more optimal method idk
    std::cout << "AHHHHHHHH " <<std::endl;
    for(int i = 0; i < channel->mNumPositionKeys; i++) {
        std::cout << "CHANNEL POSITION KEY " << i <<std::endl;
        if (timePoint < channel->mPositionKeys[i].mTime) {
            return i;
        }
    }
    return 0;
}

Mesh Model::getMesh() {
    return meshes[0];
}
std::vector<Mesh> Model::getMeshes() {
    return meshes;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        
        if(mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    } 

    //bones -- finally calc weights, buffer that ish
    for(int i = 0; i < mesh->mNumBones; i++) {
        aiBone *bone = mesh->mBones[i];
        int foundBoneIdx = 0; // match bone back to array of bone nodes we made earlier
        bool boneFound = false;

        for(int j = 0; j < this->bones.size(); j++) { // shitty but idgaf
            if(strcmp(this->bones[j].name.c_str(), bone->mName.C_Str())) {
                boneFound = true;
                foundBoneIdx = j;
                break;
            }
        }

        this->bones[i].offset = aiMatrix4x4ToGlm(bone->mOffsetMatrix);

        for(int j = 0; j < bone->mNumWeights; j++) {
            int vertId = bone->mWeights[j].mVertexId;
            float weight = bone->mWeights[j].mWeight;
            for(int k = 0; k < 4; k++) { // max bones per vert -- 4 rn -- idk why there wold be more than 1. probklem for later
                if(vertices[vertId].boneWeights[k] == 0) {
                    vertices[vertId].boneIdxs[k] = foundBoneIdx;
                    vertices[vertId].boneWeights[k] = weight;
                }
            }
        }
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::loadBones(const aiScene *scene) {
    aiNode *rootBone = findRootBone(scene);
    if(!rootBone) {
        return; // no bones
    }
    processBoneTree(rootBone, 0);
}

aiNode* Model::findRootBone(const aiScene *scene) {
    aiNode *rootNode = scene->mRootNode;

    for(int i = 0; i < rootNode->mNumChildren; i++) {
        aiNode *node = rootNode->mChildren[i];
        if(isBoneOnlyNode(node)) {
            return node;
        }
    }
    return nullptr;
}

bool Model::isBoneOnlyNode(aiNode *node) {
    if(node->mNumMeshes > 0) {
        return false;
    } // ah yes, this is a bone
    bool found = true;
    for(int i = 0; i < node->mNumChildren; i++) {
        if(!isBoneOnlyNode(node->mChildren[i])) {
            found = false;
        }
    }
    return found;
}

void Model::processBoneTree(aiNode *node, int parentIdx) {
    Bone bone;
    int newBoneIdx = numBones;
    // bone.id = 0; not needed -- idx is the id lol
    bone.name = node->mName.C_Str();
    bone.parentId = parentIdx;
    numBones++;
    bones.push_back(bone);
    for(int i = 0; i < node->mNumChildren; i++) {
        processBoneTree(node->mChildren[i], newBoneIdx);
    }
}

bool Model::isLoaded() {
    return !this->meshes.empty();
}