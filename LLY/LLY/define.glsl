struct Light{
	bool enable;
	int type;
	vec3 position;
	vec3 color;
	float intensity;
	float constant;
	float linear;
	float quadratic;		   
	vec3 cone_direction;
	float cut_off;
	float exponent;
};