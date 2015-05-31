#version 430 core
uniform sampler2D u_diffuse_tex;

in float v_depth;
in vec3 v_normal;
in vec2 v_texcoord;

layout(location = 0) out vec4 o_gb1;  
layout(location = 1) out vec4 o_gb2; 
layout(location = 2) out vec4 o_gb3;
layout(location = 3) out vec4 o_gb4;

void main()
{
	vec4 gb1 = vec4(texture2D(u_diffuse_tex, v_texcoord).rgb, 1.0);
	vec4 gb2 = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 gb3 = vec4((v_normal.xy + vec2(1.0)) * 0.5, 0.0, 0.0);
	vec4 gb4 = vec4(v_depth);
	
	o_gb1 = gb1;
	o_gb2 = gb2;
	o_gb3 = gb3;
	o_gb4 = gb4;
}
