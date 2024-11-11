#version 400

in vec3 vertex_position;

uniform mat4 M;


void main()
{
    gl_Position = M * vec4(vertex_position, 1.0);
}