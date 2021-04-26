#version 330 core

uniform vec2 u_topLeft;
uniform vec2 u_size;
uniform float u_layer;
uniform vec4 u_color;

out vec4 color;

void main()
{
    color = u_color;
}