#version 330 core 

in vec2 TexCoord;

out vec4 outColor;
uniform sampler2D ourTexture;

void main()
{	
	outColor = vec4(texture(ourTexture, TexCoord).xyza);
}