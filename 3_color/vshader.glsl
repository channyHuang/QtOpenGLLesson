#version 430 core

in vec3 posVertex;
in vec3 colVertex;
out vec4 col;

void main(void)
{
    gl_Position = vec4(posVertex, 1.0f);
    col = vec4(colVertex, 1.0f);
}

