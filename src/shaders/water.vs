#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

uniform vec3 playerPos;

void main()
{
    TexCoords = aTexCoords;    
    normal = mat3(transpose(inverse(model))) * aNormal;
    fragPos = vec3(model * vec4(aPos, 1.0)); 
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    if(distance(fragPos.xz, playerPos.xz) > .1) {
        gl_Position.y += (
                sin((time * 2) + ((5.0 * fragPos.x) + (time * fragPos.z))) +
                sin((time * 2) + ((4.0 * fragPos.x) + (time * fragPos.z))) +
                sin((time * 2) + ((time * fragPos.x) + (3.0 * fragPos.z))) +
                sin((time * 2) + ((time * fragPos.x) + (2.0 * fragPos.z))) +
                sin((time * 2) + ((fragPos.x) + (fragPos.z)))
        ) * .05;
    }
}