#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

out vec2 TexCoords;

uniform mat4 MVP;

void main()
{
    TexCoords = VertexTexCoords;
    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}
