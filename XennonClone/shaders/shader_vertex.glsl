#version 330 core

in vec2 position;
in vec2 texCoord;

uniform mat4 model;
uniform vec2 UVTileSize;
uniform vec2 UVTileOffset;

out vec2 TexCoord;

void main()
{
    TexCoord = texCoord;
        
    gl_Position = model * vec4(position, 1.0, 1.0);
}