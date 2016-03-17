#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// GLSL Hacker automatic uniforms:
uniform mat4 P;

in Vertex {
    
    vec4 color;
    float size;
} vertex[];

out vec2 PositionOnQuad;
out vec4 Color;

void main ()
{
    vec4 Position = gl_in[0].gl_Position;
    
    // a: left-bottom
    vec2 va = Position.xy + vec2(-0.5, -0.5) * vertex[0].size;
    gl_Position = P * vec4(va, Position.zw);
    PositionOnQuad = vec2(-1.0, -1.0);
    Color = vertex[0].color;
    EmitVertex();
    
    // d: right-bottom
    vec2 vd = Position.xy + vec2(0.5, -0.5) * vertex[0].size;
    gl_Position = P * vec4(vd, Position.zw);
    PositionOnQuad = vec2(1.0, -1.0);
    Color = vertex[0].color;
    EmitVertex();
    
    // b: left-top
    vec2 vb = Position.xy + vec2(-0.5, 0.5) * vertex[0].size;
    gl_Position = P * vec4(vb, Position.zw);
    PositionOnQuad = vec2(-1.0, 1.0);
    Color = vertex[0].color;
    EmitVertex();
    
    // c: right-top
    vec2 vc = Position.xy + vec2(0.5, 0.5) * vertex[0].size;
    gl_Position = P * vec4(vc, Position.zw);
    PositionOnQuad = vec2(1.0, 1.0);
    Color = vertex[0].color;
    EmitVertex();  
    
    EndPrimitive();  
}