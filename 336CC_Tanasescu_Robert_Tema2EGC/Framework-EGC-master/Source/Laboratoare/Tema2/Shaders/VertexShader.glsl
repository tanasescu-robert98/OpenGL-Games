#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

uniform vec3 culoare_lumina;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_pos = (Model * vec4(v_position,1)).xyz;
	vec3 N = normalize( mat3(Model) * v_normal );

	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);
	vec3 R = reflect(-L,N);

	// TODO: define ambient light component
	float ambient_light = material_kd * 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max(0, dot(N,L));

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light =  material_ks * pow(max(0, dot(N,H)), material_shininess);
	}

	// TODO: compute light
	float d = length(world_pos - light_position);
	float factor_atenuare = 1/(pow(d,2));
	vec3 culoare = object_color * (ambient_light + culoare_lumina * (diffuse_light  + specular_light ) * (1/(pow(d,2))));

	// TODO: send color light output to fragment shader
	//color = vec3(1);
	color = culoare;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
