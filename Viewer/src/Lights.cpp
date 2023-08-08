#include "Lights.h"
Lights::Lights()
{
}
Lights::~Lights()
{
}
Lights::Lights(float R_ambient, float G_ambient, float B_ambient, float R_specular, float G_specular, float B_specular, float R_diffuse, float G_diffuse, float B_diffuse)
{
	this->ambient_color.x = R_ambient;
	this->ambient_color.y= G_ambient;
	this->ambient_color.z = B_ambient;
	this->specular_color.x = R_specular;
	this->specular_color.y = G_specular;
	this->specular_color.z = B_specular;
	this->diffuse_color.x = R_diffuse;
	this->diffuse_color.y= G_diffuse;
	this->diffuse_color.z = B_diffuse;
	//location[3][0] = -INFINITY;
	//location[3][0] = 600;
	//location[3][1] = 300;
}
glm::vec3 Lights::GetColorVec(int index)
{
	if (index == 0)
		return ambient_color;
	if (index == 1)
		return specular_color;
	return diffuse_color;
}
glm::vec3 Lights::GetLocationVec()
{
	glm::vec3 Location = glm::vec3(location.x, location.y, location.z);
	return Location;
}
