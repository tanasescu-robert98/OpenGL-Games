#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float factor_deformare;
uniform vec3 culoare_bila = vec3(0.0, 0.5, 0.0);


out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

void main()
{
	float deformare = 0.1 * sin(7 * factor_deformare * v_position.z);
	frag_normal = v_normal;
	//frag_color = culoare_bila;
	frag_color = v_position + deformare;
	tex_coord = v_texture_coord;
	gl_Position = Projection * View * Model * vec4(v_position+deformare, 1.0);
}
