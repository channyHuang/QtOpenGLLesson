#version 430 core

in vec3 posVertex;
in vec3 colVertex;
out vec4 col;
uniform mat4 mvp_matrix;

void main(void)
{
    gl_Position = mvp_matrix * vec4(posVertex * 0.5f, 1.0f);
    col = vec4(colVertex, 1.f);
}

