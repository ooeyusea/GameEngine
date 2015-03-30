vec3 ambient(vec3 ka, vec3 color)
{
	return ka * color;
}

vec3 diffuse(vec3 kd, vec3 direction, vec3 normal, vec3 color)
{
	return kd * color * max(0.0, dot(normal, direction));
}

vec3 specular(vec3 ks, vec3 direction, vec3 normal, vec3 color, vec3 view, float shininess)
{
	return ks * color * pow(max(0.0, dot(normal, normalize(direction + view))), shininess);
}

vec3 do_direct_light(vec3 color, 
                     vec3 normal, 
					 vec3 position, 
					 vec3 eye, 
					 float shininess, 
					 vec3 light_direction, 
					 vec3 light_color,
					 float intensity)
{
	return (diffuse(vec3(1.0), light_direction, normal, light_color) * color 
		 + specular(vec3(1.0), light_direction, normal, light_color, normalize(eye - position), shininess)) * intensity;
}

vec3 do_point_light(vec3 color, 
                    vec3 normal, 
					vec3 position, 
					vec3 eye, 
					float shininess, 
					vec3 light_position, 
					vec3 light_color,
					float intensity,
					float constant_attenuation,
					float linear_attenuation,
					float quadratic_attenuation)
{
	vec3 light_direction = light_position - position;
	float distance = length(light_direction);
	light_direction = normalize(light_direction);
	
	float attenuation = 1.0 / (constant_attenuation + linear_attenuation * distance + quadratic_attenuation * distance * distance);
	
	return (diffuse(vec3(1.0), light_direction, normal, light_color) * color 
		 + specular(vec3(1.0), light_direction, normal, light_color, normalize(eye - position), shininess)) * intensity * attenuation;
}

vec3 do_spot_light(vec3 color, 
				   vec3 normal, 
				   vec3 position, 
				   vec3 eye, 
				   float shininess,
				   vec3 light_position,
				   vec3 light_color,
				   float intensity,
				   float constant_attenuation,
				   float linear_attenuation,
				   float quadratic_attenuation,
				   vec3 cone_direction,
				   float cut_off,
				   float exponent)
{
	vec3 light_direction = light_position - position;
	float distance = length(light_direction);
	light_direction = normalize(light_direction);
	
	float attenuation = 1.0 / (constant_attenuation + linear_attenuation * distance + quadratic_attenuation * distance * distance);
	
	float spot_cos = dot(light_direction, -cone_direction);
	if (spot_cos < cut_off)
		attenuation = 0.0;
	else
		attenuation *= pow(spot_cos, exponent);
	
	return (diffuse(vec3(1.0), light_direction, normal, light_color) * color 
		 + specular(vec3(1.0), light_direction, normal, light_color, normalize(eye - position), shininess)) * intensity * attenuation;
}

vec4 lighting(vec4 color, vec3 normal, vec3 position, vec3 eye, float shininess)
{
	vec3 ret = ambient(vec3(1.0), LLY_Ambient) * color.rgb;
	for (int i = 0; i < 20; ++i)
	{
		if (!LLY_Lights[i].enable)
			continue;
		
		switch (LLY_Lights[i].type)
		{
		case 0: ret += do_direct_light(color.rgb, normal, position, eye, shininess, 
		                               -LLY_Lights[i].position, LLY_Lights[i].color, LLY_Lights[i].intensity); break;
		case 1: ret += do_point_light(color.rgb, normal, position, eye, shininess, 
		                              -LLY_Lights[i].position, LLY_Lights[i].color, LLY_Lights[i].intensity,
									  LLY_Lights[i].constant, LLY_Lights[i].linear, LLY_Lights[i].quadratic); break;
		case 2: ret += do_spot_light(color.rgb, normal, position, eye, shininess, 
		                             -LLY_Lights[i].position, LLY_Lights[i].color, LLY_Lights[i].intensity,
									 LLY_Lights[i].constant, LLY_Lights[i].linear, LLY_Lights[i].quadratic,
									 LLY_Lights[i].cone_direction,
									 LLY_Lights[i].cut_off,
									 LLY_Lights[i].exponent); break;
		}
	}
	return vec4(ret, color.a);
}

vec4 look_up(vec4 color)
{
	if (LLY_UseColorTable)
	{
		vec4 tmp = texture3D(LLY_ColorTable, color.rgb);
		tmp.a = color.a;
		return tmp;
	}
	return color;
}