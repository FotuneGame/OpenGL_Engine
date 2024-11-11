#version 400

in vec3 vertex_position;

uniform mat4 V;
uniform mat4 P;

out vec3 texCoord;

void main()
{
    vec4 MVP_Pos = P*V * vec4 (vertex_position, 1.0);
    gl_Position = MVP_Pos.xyww;
    texCoord = vertex_position;
}