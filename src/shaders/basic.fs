#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool selected;

void main()
{   
    vec4 diffuse = texture(texture_diffuse1, TexCoords);

    if(diffuse.a < 0.1) {
        discard;
    }

    // if(selected) {
    //     float borderThicknessX = 4.0 / 800;  
    //     float borderThicknessY = 4.0 / 600;  

    //     if(TexCoords.x < borderThicknessX || TexCoords.x > 1.0 - borderThicknessX) {
    //         float c = sin(TexCoords.x * 50 + TexCoords.y * 50);
    //         diffuse = vec4(0.0, 1.0 - c, 0.0, 1.0 - c);
    //     }

    //     if(TexCoords.y < borderThicknessY || TexCoords.y > 1.0 - borderThicknessY) {
    //         float c = sin(TexCoords.x * 50 + TexCoords.y * 50);
    //         diffuse = vec4(0.0, 1.0 - c, 0.0, 1.0 - c);
    //     }   
    // }

    FragColor = diffuse;
}