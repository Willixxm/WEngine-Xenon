#version 330 core 

in vec2 TexCoord;

out vec4 outColor;
uniform sampler2D ourTexture;

void main()
{	
	outColor = vec4(texture(ourTexture, TexCoord).xyza);
	if (outColor == vec4(1, 0, 1, 1))
		discard;
		
}