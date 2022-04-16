#version 430 core

in vec3 posVertex;
in vec2 posTexture;
uniform mat4 mvp_matrix;

out vec2 v_texcoord;

void main(void)
{
    gl_Position = mvp_matrix * vec4(posVertex * 0.5f, 1.0f);
    v_texcoord = posTexture;
}

