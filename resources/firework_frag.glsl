#version 330

in vec2 PositionOnQuad;
in vec4 Color;
out vec4 FragColor;
void main ()
{
    float dis = distance(PositionOnQuad, vec2(0));
    float alpha = max(1 - (dis * dis), 0);
    vec3 newColor = vec3(1) * (alpha) + Color.xyz * (1 - alpha);
    FragColor = vec4(newColor, alpha);
}