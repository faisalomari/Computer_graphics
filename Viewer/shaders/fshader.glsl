#version 330 core

struct Material{
	sampler2D textureMap;
	vec3 k_i;
	vec3 k_d;
	vec3 k_s;
	
};
struct Light{
	vec3 l_i;
	vec3 l_d;
	vec3 l_s;
	vec3 pos;
	float alpha;
};

uniform Material material;
uniform Light light;
uniform vec3 camera_pos;
uniform int type;

in vec2 frag_tex_coords;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 frag_color;

float levels=2 ;//two different levels of shading 

void main()
{
	vec3 textureColor = vec3(texture(material.textureMap, frag_tex_coords));
	
	
	vec3 Ia = material.k_i * light.l_i;

	float cosQ = max(dot(normalize(fragNormal), light.pos - fragPos), 0.0);

	vec3 I = normalize( light.pos - fragPos);

	vec3 r = I - 2 * dot(I, normalize(fragNormal)) * normalize(fragNormal);

	vec3 Id = material.k_d * cosQ * light.l_d;

	vec3 v = fragPos - camera_pos;

	cosQ = max(dot(r, v), 0.0);

	vec3 Is = material.k_s * pow(cosQ, light.alpha) * light.l_s;;


	float level=floor(pow(cosQ, light.alpha) * levels);
	float brightness=level/levels;

	vec4 temp_color=vec4(Is+Id+Ia,1);
	if(type==-1){
		frag_color=vec4(Is+Id+Ia,1);
		frag_color = texture(material.textureMap, frag_tex_coords);
	}
	else if(type==1){//normal mapping
		vec3 normal = vec3(texture(material.textureMap, frag_tex_coords)).rgb;
		normal = normalize(normal * 2.0 - 1.0);   
		vec3 I_A = (textureColor * light.l_i) ;
		float temp_cosQ = max(dot(normalize(normal), light.pos - fragPos), 0.0);
		vec3 I_D = material.k_d * temp_cosQ * light.l_d;
		float temp_q=max(dot(r, v), 0.0);
		vec3 I_S =  light.l_s * pow(temp_q, light.alpha) * material.k_s;
		vec3 final_light = (I_A + I_D + I_S);
		frag_color=vec4(final_light,1);
	}
}
