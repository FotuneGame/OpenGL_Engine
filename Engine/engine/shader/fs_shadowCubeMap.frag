#version 400

in vec4 f_pos;

uniform vec3 light_pos;
uniform float far_plane;


void main()
{
    gl_FragDepth =  length(f_pos.xyz - light_pos) / far_plane;
}  
