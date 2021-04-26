#version 330 core

layout (location = 0) in vec4 vertex;

uniform sampler2D u_atlas;
uniform mat4 u_mat;

out vec2 v_texCoord;

void main()
{
    gl_Position = u_mat * vec4(vertex.xy, 0, 1);
    v_texCoord  = vertex.zw;
}