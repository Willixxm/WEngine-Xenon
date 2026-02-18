#version 330 core 

in vec2 TexCoord;

out vec4 outColor;
uniform sampler2D ourTexture;

void main()
{	
	
	outColor = vec4(texture(ourTexture, TexCoord).xyz, 1.0);
	//outColor = vec4(TexCoord, 0.0, 1.0);

	// Discard magenta pixels (R high, G low, B high)
    if (outColor.r > 0.8 && outColor.g < 0.2 && outColor.b > 0.8)
        discard;
}