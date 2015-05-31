#version 430 core

uniform int u_light_type;
uniform vec4 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_light_direction;
uniform vec3 u_light_attenuation;
uniform vec2 u_light_cutoff;

uniform sampler2D g_buffer1;
uniform sampler2D g_buffer2;
uniform sampler2D g_buffer3;
uniform sampler2D g_buffer4;

uniform mat4 inverse_viewproject;
uniform vec3 u_eye;

in vec2 v_texcoord;
out vec4 fColor;

void cal_ambient_light(vec4 diffuse)
{
	fColor = u_light_color * diffuse;
}

vec4 cal_blinn_phong(vec3 normal, vec3 light, vec3 view, vec4 color, vec4 diffuse, vec4 specular, float shinness)
{
	vec4 ret = vec4(0.0, 0.0, 0.0, 1.0);
	
	float ndotl = dot(normal, -light);
	if (ndotl > 0)
	{
		ret = diffuse * color * ndotl;
		
		vec3 h = normalize(view - light);
		float ndoth = dot(normal, h);
		if (ndoth > 0)
		{
			ret += specular * color * pow(ndoth, shinness);
		}
	}
	return ret;
}

void cal_direction_light(vec3 normal, vec3 position, vec4 diffuse, vec4 specular, float shinness)
{
	vec3 view = (u_eye - position);
	fColor = cal_blinn_phong(normal, normalize(u_light_direction), normalize(view), u_light_color, diffuse, specular, shinness);
}

void cal_point_light(vec3 normal, vec3 position, vec4 diffuse, vec4 specular, float shinness)
{
	vec3 view = (u_eye - position);
	float distance = length(view);
	float attenuation = 1.0 / (u_light_attenuation.x + u_light_attenuation.y * distance + u_light_attenuation.z * distance * distance);
	
	fColor = cal_blinn_phong(normal, normalize(position - u_light_position), normalize(view), u_light_color, diffuse, specular, shinness) * attenuation;
}

void cal_spot_light(vec3 normal, vec3 position, vec4 diffuse, vec4 specular, float shinness)
{
	vec3 light_direction = normalize(position - u_light_position);
	
	float spot = pow(dot(normalize(u_light_direction), light_direction), u_light_cutoff.x);
	if (spot > u_light_cutoff.y)
	{
		vec3 view = (u_eye - position);
		float distance = length(view);
		float attenuation = spot / (u_light_attenuation.x + u_light_attenuation.y * distance + u_light_attenuation.z * distance * distance);
		
		fColor = cal_blinn_phong(normal, normalize(light_direction), normalize(view), u_light_color, diffuse, specular, shinness) * attenuation;
	}
}

void main()
{
	vec4 gb1 = texture2D(g_buffer1, v_texcoord);
	vec4 gb2 = texture2D(g_buffer2, v_texcoord);
	vec4 gb3 = texture2D(g_buffer3, v_texcoord);
	vec4 gb4 = texture2D(g_buffer4, v_texcoord);
	
	vec4 diffuse = vec4(gb1.rgb, 1.0);
	vec4 specular = vec4(gb2.ba, gb3.b, 1.0);
	vec3 normal;
	normal.xy = gb3.rg * 2.0 - vec2(1.0);
	normal.z = sqrt(1.0 - dot(normal.xy, normal.xy));
	float shinness = gb3.a;
	
	vec4 projected_position = vec4(v_texcoord.x * 2.0 - 1.0, (1.0 - v_texcoord.y) * 2.0 - 1.0, gb4.x, 1.0);
	vec4 position_t = inverse_viewproject * projected_position;
	vec3 position = position_t.xyz / position_t.w;
	
	if (u_light_type == 1)
		cal_ambient_light(diffuse);
	else if (u_light_type == 2)
		cal_direction_light(normal, position, diffuse, specular, shinness);
	else if (u_light_type == 3)
		cal_point_light(normal, position, diffuse, specular, shinness);
	else if (u_light_type == 4)
		cal_spot_light(normal, position, diffuse, specular, shinness);
}
