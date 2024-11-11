#version 400

in vec3 vertex_position;
in vec3 vertex_color;
in vec3 vertex_normal;
in vec2 vertex_texture;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 TM;

uniform mat4 LV_DIR;
uniform mat4 LP_DIR;

out vec3 normal;
out vec3 f_pos;
out vec3 color;
out vec2 tex_coord;
out vec4 f_pos_light_space;

void main () {
	normal = mat3(transpose(inverse(M))) * vertex_normal;
	f_pos = vec3(M * vec4(vertex_position, 1.0));
	gl_Position = P*V*M * vec4 (vertex_position, 1.0);
	color = vertex_color;
	tex_coord = (TM * vec4(vertex_texture, 1.0, 1.0)).xy;
	f_pos_light_space = LP_DIR * LV_DIR * vec4(f_pos, 1.0);
}