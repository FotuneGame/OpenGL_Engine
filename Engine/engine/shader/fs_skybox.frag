#version 400

in vec3 texCoord;
out vec4 frag_color;

uniform vec3 campos;
uniform samplerCube cubemap;

void main()
{
    frag_color = texture(cubemap, texCoord);
}
