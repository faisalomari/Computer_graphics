#pragma once
#include <glm/glm.hpp>
class Lights
{
public:
	Lights();
	~Lights();
	Lights(float R_ambient, float G_ambient, float B_ambient, float R_specular, float G_specular, float B_specular, float R_diffuse, float G_diffuse, float B_diffuse);
	glm::vec3 GetColorVec(int index);
	glm::vec3 GetLocationVec();
	float alpha = 1;
	glm::vec3 location = glm::vec3(0.0f);
	int light_type = 0;
	glm::vec3 ambient_color = glm::vec3(1,0,0);
	glm::vec3 specular_color = glm::vec3(1.0f);
	glm::vec3 diffuse_color = glm::vec3(1.0f);
	//int directional_light = 1;
};



