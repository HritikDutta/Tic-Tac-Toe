#version 330 core

in vec2 v_texCoord;

out vec4 color;

uniform sampler2D u_atlas;
uniform mat4 u_mat;

void main()
{
    color = texture(u_atlas, v_texCoord);
}