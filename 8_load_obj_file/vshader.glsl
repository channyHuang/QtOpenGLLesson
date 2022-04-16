#version 430 core

in vec3 posVertex;
in vec3 norVertex;
in vec2 posTexture;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat3 normal_matrix;

//out vec2 v_texcoord;
//out vec3 vert;
//out vec3 vertNormal;

void main(void)
{
    //vert = posVertex;
    //vertNormal = normal_matrix * norVertex;
    gl_Position = proj_matrix * mv_matrix * vec4(posVertex, 1.0f);
    //v_texcoord = posTexture;
}

