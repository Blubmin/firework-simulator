#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in float Size;

// GLSL Hacker automatic uniforms:
uniform mat4 MV;

out Vertex {
    out vec4 color;
    out float size;
} vertex;

void main()
{
    gl_Position = MV * vec4(Position, 1.0);
    vertex.color = vec4(Color, 1.0);
    vertex.size = Size;
}