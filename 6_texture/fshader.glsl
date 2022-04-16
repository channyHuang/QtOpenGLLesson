#version 430 core

uniform sampler2D texture;
in vec2 v_texcoord;
out vec4 pre_gl_FragColor;

void main(void)
{
    //gl_FragColor = texture2D(texture, v_texcoord);
    pre_gl_FragColor = texture2D(texture, v_texcoord);
}
