#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

vec3 texsample(const float x, const float y, vec2 fragCoord)
{
    vec2 uv = fragCoord.xy;
	uv = (uv + (uv * vec2(x, y)));
	return texture(screenTexture, uv).xyz;
}

vec3 texfilter(vec2 fragCoord)
{
    float offset = 1.0 / 250.0;
    vec3 sum = texsample(-offset, -offset, fragCoord) * -1.
             + texsample(-offset,  0, fragCoord) * -1.
             + texsample(-offset,  offset, fragCoord) * -1.
             + texsample( 0, -offset, fragCoord) * -1.
             + texsample( 0,  0, fragCoord) * 9
             + texsample( 0,  offset, fragCoord) * -1.
             + texsample( offset, -offset, fragCoord) * -1.
             + texsample( offset,  0, fragCoord) * -1.
             + texsample( offset,  offset, fragCoord) * -1.;
    
	return sum;
}

void main()
{ 
    //FragColor = texture(screenTexture, TexCoords);
    //FragColor = texture(screenTexture, TexCoords) * vec4(1.0, 1.0 - pow(distance(TexCoords, vec2(0.5)),2.0), 1.0 - pow(distance(TexCoords, vec2(0.5)),2.0), 1.0);

    vec3 cf = texfilter(TexCoords);
    // vec3 cl = texsample(0, 0, TexCoords);
    FragColor = vec4(cf, 1.0);
    // working blur
    // float kernelSize = 15.0;
    // int halfSize = 7;
    // float coeff = 1.0 / (kernelSize * kernelSize);
    // vec2 dx = vec2(.002, 0.0);
    // vec2 dy = vec2(0.0, 0.002);

    // FragColor = vec4(0.0);
    // for(int x = -halfSize; x <= halfSize; x++) {
    //     for(int y = -halfSize; y <= halfSize; y++) {
    //         FragColor += coeff * texture(screenTexture, TexCoords + x * dx + y * dy);
    //     }
    // }
    //const float offset = 1.0; 

    // vec2 offsets[9] = vec2[](
    //     vec2(-offset, -offset), // bottom-left
    //     vec2(-offset,  0.0f),   // center-left
    //     vec2(-offset,  offset), // top-left
    //     vec2( 0.0f,   -offset), // bottom-center
    //     vec2( 0.0f,    0.0f),   // center-center
    //     vec2( 0.0f,    offset), // top-center
    //     vec2( offset, -offset),  // bottom-right   
    //     vec2( offset,  0.0f),   // center-right
    //     vec2( offset,  offset) // top-right
    // );

    // float kernel[9] = float[](
    //     0, -1, 0,
    //     -1,  9, -1,
    //     0, -1,0
    // );
    
    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++)
    // {
    //     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    // }
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++)
    //     col += sampleTex[i] * kernel[i];
    
    // FragColor = vec4(col, 1.0);
}