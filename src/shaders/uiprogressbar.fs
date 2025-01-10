#version 330
in vec2 texCoords;
uniform float progress;
uniform sampler2D uiTex;
uniform vec2 screenSize;
out vec4 color;

float rand(vec2 st) {
    return fract(sin(dot(st, vec2(36.164927, 44.1028472))) * 64637.7890);
}

void main() {
        //color = vec4(0.0,1.0,0.0,1.0);
    vec4 diffuse = texture(uiTex, texCoords);
    vec2 st = gl_FragCoord.xy / screenSize;
    if(texCoords.y < progress) {
        color = diffuse;
    } else { // todo: noise
        st *= 100.0;
        vec3 noiseColor = vec3(rand(floor(st)));
        color = vec4(noiseColor, 1.0);
    }

    float borderThicknessX = 5.0 / screenSize.x;  
    float borderThicknessY = 5.0 / screenSize.y;  

    if(texCoords.x < borderThicknessX || texCoords.x > 1.0 - borderThicknessX || texCoords.y < borderThicknessY || texCoords.y > 1.0 - borderThicknessY) {
        color = vec4(1.0, 0.0, 0.0, 1.0);
    }
}