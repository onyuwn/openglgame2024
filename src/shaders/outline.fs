#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform float curTime;

void main() {
    vec4 color = vec4(0.0);
    float c = sin(TexCoords.x * 50 + TexCoords.y * 50 + curTime*2);
    color = vec4(0.0, 1.0 - c, 0.0, 1.0 - c); 
    FragColor = color;
}