#version 130
in vec3 vert;
in vec3 vertNormal;
in vec3 vertColor;
out vec4 fragColor;
uniform vec3 lightPos;

void main() {
    vec3 L = normalize(lightPos - vert);
    float NL = max(abs(dot(normalize(vertNormal), L)), 0.0);
    vec3 col = clamp(vertColor * 0.2 + vertColor * 0.8 * NL, 0.0, 1.0);
    fragColor = vec4(col, 1.0);
}
