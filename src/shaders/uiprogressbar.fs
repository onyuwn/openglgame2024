#version 330
in vec2 texCoords;
uniform float progress;
out vec4 color;

void main() {
        //color = vec4(0.0,1.0,0.0,1.0);

    if(texCoords.x < progress) {
        color = vec4(0.0,1.0,0.0,1.0);
    } else {
        color = vec4(1.0, 0.0, 0.0, 1.0);
    }
}