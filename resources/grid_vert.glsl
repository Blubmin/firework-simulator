#version 330

layout(location = 0) in vec3 Position;

uniform mat4 MV;
uniform mat4 P;

void main() {
    vec4 pos = P * MV * vec4(Position, 1.0);
    gl_Position = pos;
}