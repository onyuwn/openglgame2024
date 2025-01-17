#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in uvec4 bone_idxs;
layout (location = 4) in vec4 bone_weights;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 bone_matrices[4];

vec3 outPos;

void main()
{
   /* mat4 boneTransform = mat4(0.0);
    bool hasBones = false;

    for(int i = 0; i < 4; i++) {
        if(bone_weights[i] > 0) {
            hasBones = true;
            boneTransform += bone_matrices[bone_idxs[i]] * bone_weights[i];
        }
    }

    if(hasBones) {
        outPos = vec3(model * boneTransform * vec4(aPos, 1.0));
    } else {
        outPos = aPos;
    }*/

    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}