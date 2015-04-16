#version 430 core
uniform sampler2D u_diffuse_tex;
uniform vec4 u_ambient;

in vec2 v_texcoord;

vec4 cal_ambient(vec4 ka)
{
	return u_ambient * ka;
}

vec4 cal_lighting(vec4 kd, vec4 ks, float shinness, vec3 normal, vec3 view, vec4 light_color, vec3 light_dir)
{
	vec4 color = max(vec4(0), light_color * kd * dot(normal, light_dir));
	
	vec3 h = normalize(view + light_dir);
	color += light_color * ks * pow(max(0, dot(h, normal)), shinness);

	return color;
}

out vec4 fColor;

void main()
{
	vec4 color = texture2D(u_diffuse_tex, v_texcoord);
	color.a = 1.0;
	fColor = cal_ambient(color);
}
