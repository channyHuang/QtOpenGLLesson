#version 430 core

varying highp vec3 vert;
varying highp vec3 vertNormal;
uniform highp vec3 lightPos;

uniform sampler2D texture;
varying vec2 v_texcoord;

void main(void)
{
    highp vec3 L = normalize(lightPos - vert);
    highp float NL = max(dot(normalize(vertNormal), L), 0.0);
    highp vec4 color = texture2D(texture, v_texcoord);
    highp vec4 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);

    gl_FragColor = col;
}
