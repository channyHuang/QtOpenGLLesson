#version 430 core

//in highp vec3 vert;
//in highp vec3 vertNormal;
//in vec2 v_texcoord;

uniform highp vec3 lightPos;
uniform sampler2D texture;

in vec4 col;
out vec4 pre_gl_FragColor;

void main(void)
{
    //highp vec3 L = normalize(lightPos - vert);
    //highp float NL = max(dot(normalize(vertNormal), L), 0.0);
    //highp vec4 color = texture2D(texture, v_texcoord);
    //highp vec4 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);
    //highp vec4 col = color * 0.2f + color * NL * 0.8f;
    //col = normalize(col);

    //gl_FragColor = col;
    pre_gl_FragColor = col;//vec4(1.f, 0.f, 0.f, 1.f);
}
