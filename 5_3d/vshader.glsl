#version 430 core

in vec3 posVertex;
in vec3 colVertex;
out vec4 col;
uniform mat4 matrix;

void main(void)
{
    gl_Position = matrix * vec4(posVertex, 1.0f);
    col = vec4(colVertex, 1.0f);
}

