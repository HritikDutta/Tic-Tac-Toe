#version 330 core

uniform vec2 u_topLeft;
uniform vec2 u_size;
uniform float u_layer;
uniform vec4 u_color;

void main()
{
    vec2 vertices[] = vec2[](
        vec2(0, 0),
        vec2(0, 1),
        vec2(1, 0),
        vec2(1, 1)
    );

    vec4 position = vec4(vertices[gl_VertexID], u_layer, 1);
    position.xy *= u_size;
    position.y *= -1;
    position.xy += u_topLeft;

    gl_Position = position;
}