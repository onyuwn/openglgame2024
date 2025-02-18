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
    float offset = 1.0 / 300.0; // yuh
    vec3 sum = texsample(-offset, -offset, fragCoord) * 0.
             + texsample(-offset,  0, fragCoord) * -1.
             + texsample(-offset,  offset, fragCoord) * 0.
             + texsample( 0, -offset, fragCoord) * -1.
             + texsample( 0,  0, fragCoord) * 5
             + texsample( 0,  offset, fragCoord) * -1.
             + texsample( offset, -offset, fragCoord) * 0
             + texsample( offset,  0, fragCoord) * -1.
             + texsample( offset,  offset, fragCoord) * 0;
    
	return sum;
}

void main()
{ 
    //FragColor = texture(screenTexture, TexCoords);

    vec3 cf = texfilter(TexCoords);
    FragColor = vec4(cf, 1.0);
}