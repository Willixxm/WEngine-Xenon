#version 330 core

in vec3 position;

uniform texCoord;
uniform mat4 model;

out vec3 fragPosition;
out vec3 fragNormal;
out vec3 LightPosition;

out vec2 TexCoord;

void main()
{
    TexCoord = texCoord;


    gl_Position = model * vec4(position, 1.0);

}