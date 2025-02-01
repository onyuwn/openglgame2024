#version 330

out vec4 fragColor;
uniform vec4 inColor;
in vec2 texCoords;
uniform sampler2D uiTex;
uniform float dHoverTime;
uniform float animTime;

void main() {
    if(inColor.a > 0) {
        vec4 color = inColor;
        float colorMult = 1.0 - pow(distance(texCoords, vec2(texCoords.x, 1.0)), 4.0);
        color *= vec4(colorMult, colorMult, colorMult, 1.0);

        float hoverColor = (dHoverTime/animTime);

        if(hoverColor < 1.0) {
            color += vec4(hoverColor * .3,hoverColor * .3,hoverColor * .3,0.0);
        } else {
            color += vec4(1 * .3,1 * .3,1 * .3,0.0);
        }
        fragColor = color;
    } else {
        vec4 diffuse = texture(uiTex, texCoords);
        fragColor = diffuse;
    }
}