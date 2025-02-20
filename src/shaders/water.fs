#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 fragPos;

uniform float time;
uniform sampler2D texture_diffuse1;
uniform vec3 playerPos;

void main()
{   
    vec4 diffuse = texture(texture_diffuse1, TexCoords);
    float dPlayer = distance(fragPos.xz, playerPos.xz);

    if(dPlayer > 1.0) {
        vec3 waterHeight = vec3(
            (
                sin((time * 2) + ((5.0 * fragPos.x) + (time * fragPos.z))) +
                sin((time * 2) + ((4.0 * fragPos.x) + (time * fragPos.z))) +
                sin((time * 2) + ((time * fragPos.x) + (3.0 * fragPos.z))) +
                sin((time * 2) + ((time * fragPos.x) + (2.0 * fragPos.z))) +
                sin((time * 2) + ((fragPos.x) + (fragPos.z)))
            ) * .05
        );
        diffuse += vec4(waterHeight, 1.0);
    } else {
        diffuse += vec4(.25 * (dPlayer * cos(dPlayer * 20 - time * 10)));
    }

    diffuse.a = .3;
    FragColor = diffuse;
}