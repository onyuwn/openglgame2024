#version 330
in vec2 texCoords;
uniform float progress;
uniform sampler2D uiTex;
out vec4 color;

void main() {
        //color = vec4(0.0,1.0,0.0,1.0);
    vec4 diffuse = texture(uiTex, texCoords);
    if(texCoords.y < progress) {
        color = diffuse;
    } else {
        color = vec4(1.0, 0.0, 0.0, 1.0);
    }
}