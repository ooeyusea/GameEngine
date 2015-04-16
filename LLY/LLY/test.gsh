#version 430

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

void main(void) 
{
  int i;

  vec3 l1;
  vec3 l2;
  vec3 sn;

  l1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  l2 = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
  sn = cross(l1.xyz, l2.xyz);

  for(i = 0; i < gl_in.length(); i++)
  {
    gl_Position = gl_in[i].gl_Position;// + line_width * vec4(vertex[i].normal, 0.0);
    EmitVertex();
  }
  EndPrimitive();
}
