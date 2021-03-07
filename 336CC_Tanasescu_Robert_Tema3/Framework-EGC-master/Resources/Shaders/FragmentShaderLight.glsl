#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;


uniform int spotLight;

layout(location = 0) out vec4 out_color;

void main()
{

	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = reflect(L,world_normal);
	
	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	//float diffuse_light = 0;
	float diffuse_light = max(ambient_light, dot(world_normal,L));

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light =  pow(max(0, dot(world_normal,H)), material_shininess);
	}

	// TODO: compute light

	float cut_off = radians(30.);

	float d = length(world_position - light_position);
	float factor_atenuare = 1/pow(d,2);

	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);

	float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);

	vec3 objectColor;

	

	if(spotLight == 1) {
		if (spot_light > cos(cut_off)) {
			objectColor = (diffuse_light * material_kd + specular_light * material_ks) * light_att_factor * object_color;
		}
	} else  {
		objectColor = (diffuse_light * material_kd + specular_light * material_ks) * factor_atenuare * object_color;
	}

	// TODO: write pixel out color
	//out_color = vec4(1);
	out_color = vec4(objectColor, 1);
}