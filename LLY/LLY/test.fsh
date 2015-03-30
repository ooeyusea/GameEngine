#version 430 core
uniform sampler2D u_tex;

in Vertex {
  vec2 v_texcoord;
} vertex;

uniform vec4 u_color;

out vec4 fColor;

void main()
{
	fColor = u_color;// * texture2D(u_tex, vertex.v_texcoord);
}
