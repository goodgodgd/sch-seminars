#version 130
in vec3 vertex;
in vec3 normal;
in vec3 color;
in float ptsize;
out vec3 vert;
out vec3 vertNormal;
out vec3 vertColor;
uniform mat4 mvpMatrix;

void main() {
   vert = vertex;
   vertNormal = normal;
   vertColor = color;

   gl_Position = mvpMatrix * vec4(vertex,1);
   gl_PointSize = ptsize;
}
