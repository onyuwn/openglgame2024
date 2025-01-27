#include "animator.hpp"

Animator::Animator(Animation* animation) {
    mCurrentTime = 0.0;
    mCurrentAnimation = animation;

    mFinalBoneMatrices.reserve(100);
    for(int i = 0; i < 100; i++) {
        mFinalBoneMatrices.push_back(glm::mat4(1.0));
    }
}

void Animator::updateAnimation(float dt) {
    mDeltaTime = dt;

    if(mCurrentAnimation) {
        mCurrentTime += mCurrentAnimation->getTicksPerSecond() * dt;
        mCurrentTime = fmod(mCurrentTime, mCurrentAnimation->getDuration());
        calculateBoneTransform(&mCurrentAnimation->getRootNode(), glm::mat4(1.0));
    }
}

void Animator::playAnimation(Animation* pAnimation) {
    mCurrentAnimation = pAnimation;
    //mCurrentTime = 0.0f;
}

void Animator::calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform) {
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* bone = mCurrentAnimation->findBone(nodeName);

    if(bone) {
        bone->update(mCurrentTime);
        nodeTransform = bone->getLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;
    auto boneInfoMap = mCurrentAnimation->getBoneIDMap();
    if(boneInfoMap.find(nodeName) != boneInfoMap.end()) {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        mFinalBoneMatrices[index] = globalTransformation * offset;
    }

    for(int i = 0; i < node->childCount; i++) {
        calculateBoneTransform(&node->children[i], globalTransformation);
    }
}

std::vector<glm::mat4> Animator::getFinalBoneMatrices() {
    return mFinalBoneMatrices;
}