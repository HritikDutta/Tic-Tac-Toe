#version 330 core

in vec2 v_texCoord;

uniform sampler2D u_font;
uniform vec4 u_color;
uniform float u_layer;

out vec4 color;

void main()
{
    color = u_color * texture(u_font, v_texCoord).rrrr;
}