#version 400

in vec3 vertex_position;

uniform mat4 M;
uniform mat4 LP;
uniform mat4 LV;


void main()
{
    gl_Position = LP * LV * M * vec4(vertex_position, 1.0);
}