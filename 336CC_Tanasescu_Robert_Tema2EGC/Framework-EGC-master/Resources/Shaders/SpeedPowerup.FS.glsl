#version 330

in vec3 frag_color;

uniform vec3 color = vec3(0.0, 0.0, 1.0);

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(color, 1);
}