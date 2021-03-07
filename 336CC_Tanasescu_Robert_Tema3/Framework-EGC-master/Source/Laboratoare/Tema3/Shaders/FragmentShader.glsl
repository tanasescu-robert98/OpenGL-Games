#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec4 culoare1 = texture2D(texture_1, texcoord);
	vec4 culoare2 = texture2D(texture_2, texcoord);

	if(culoare1.a < 0.5f) {
		discard;
	}

	if(culoare2.a < 0.5f) {
		discard;
	}

	vec3 color = mix(culoare1.xyz, culoare2.xyz, 0.5f);
	out_color = vec4(color, 1);
	//out_color = vec4(1);
}