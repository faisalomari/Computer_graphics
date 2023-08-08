#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int mapping_type;
uniform vec3 camera_pos;

out vec3 color;
out vec2 frag_tex_coords;
out vec3 fragPos;
out vec3 fragNormal;
out vec3 reflected_vector;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

void main()
{
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;
	if(mapping_type==-1){
	    frag_tex_coords=texCoords;
	}
	else if(mapping_type==0){//planer
		frag_tex_coords.x = pos.x;
		frag_tex_coords.y = pos.y;
	}

	vec3 view_vector=normalize(gl_Position.xyz-camera_pos);
	vec3 uint_normal=normalize(fragNormal);
	reflected_vector=reflect(view_vector,uint_normal);
}