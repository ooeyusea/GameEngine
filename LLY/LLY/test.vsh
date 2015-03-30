#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 5) in vec2 vTexCoord;
layout(location = 12) in vec2 vWeight_0;
layout(location = 13) in vec2 vWeight_1;
layout(location = 14) in vec2 vWeight_2;
layout(location = 15) in vec2 vWeight_3;

uniform mat4 u_matrix[12];
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

mat4 u_aaa = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

out Vertex
{
	vec2 v_texcoord;
}vertex;

vec4 skinPosition(float blendWeight, int matrixIndex)
{
    vec4 tmp = u_matrix[matrixIndex] * vPosition;
    return blendWeight * tmp;
}

vec4 getPosition()
{
    vec4 tmp = vec4(0.0);
    tmp += skinPosition(vWeight_0.y, int(vWeight_0.x));
	tmp += skinPosition(vWeight_1.y, int(vWeight_1.x));
	tmp += skinPosition(vWeight_2.y, int(vWeight_2.x));
	tmp += skinPosition(vWeight_3.y, int(vWeight_3.x));

    return tmp;    
}

void main()
{
	vertex.v_texcoord = vTexCoord;
	gl_Position = u_projection * u_view * getPosition();
}
