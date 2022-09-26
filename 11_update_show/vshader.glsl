#version 430 core

in vec3 posVertex;
in vec3 norVertex;
in vec2 posTexture;
in vec3 colVertex;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat3 normal_matrix;

//out vec2 v_texcoord;
//out vec3 vert;
//out vec3 vertNormal;
out vec4 col;

void main(void)
{
    //vert = posVertex;
    //vertNormal = normal_matrix * norVertex;
    gl_Position = proj_matrix * mv_matrix * vec4(posVertex * 10, 1.0f);
    //v_texcoord = posTexture;
    col = vec4(colVertex, 1.f);
}

