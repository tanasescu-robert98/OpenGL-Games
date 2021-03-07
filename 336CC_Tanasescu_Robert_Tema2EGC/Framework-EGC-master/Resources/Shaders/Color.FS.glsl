#version 330

uniform vec3 color = vec3(1.000,0.833,0.224);

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(abs(color), 1);
}