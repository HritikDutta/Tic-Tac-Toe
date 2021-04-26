#version 330 core

layout (location = 0) in vec4 vertex;

uniform sampler2D u_font;
uniform vec4 u_color;
uniform float u_layer;

out vec2 v_texCoord;

void main()
{
    gl_Position = vec4(vertex.x, vertex.y, u_layer, 1);
    v_texCoord  = vertex.zw;
}