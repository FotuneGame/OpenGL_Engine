#version 400

in vec3 vertex_position;
in vec3 vertex_color;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;


out vec3 color;

void main () {
	gl_Position = P*V*M * vec4 (vertex_position, 1.0);
	color = vertex_color;
}