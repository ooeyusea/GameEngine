#version 430 core
layout(location = 0) in vec4 vPosition;

out vec2 v_texcoord;

void main()
{
	v_texcoord.x = (vPosition.x + 1.0) / 2.0;
	v_texcoord.y = (vPosition.y + 1.0) / 2.0;
	gl_Position = vPosition;
}
