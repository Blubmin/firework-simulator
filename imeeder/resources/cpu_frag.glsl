#version 120

varying vec3 fragPos; // in camera space
varying vec3 fragNor; // in camera space
varying vec3 lightPos;

uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 ambient;

void main()
{
    vec3 n = normalize(fragNor);
    vec3 l = normalize(lightPos - fragPos);
    vec3 v = -normalize(fragPos);
    vec3 h = normalize(l + v);
    vec3 colorD = max(dot(l, n), 0.0) * diffuse;
    vec3 colorS = pow(max(dot(h, n), 0.0), 200) * specular;
    vec3 color = colorD + colorS + ambient;
    gl_FragColor = vec4(color, 1.0);
}
