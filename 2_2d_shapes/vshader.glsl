#version 430 core

layout(location = 0) in vec3 posVertex;

void main(void)
{
    gl_Position = vec4(posVertex, 1.0f);
}

