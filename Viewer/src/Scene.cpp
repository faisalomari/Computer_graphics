#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	active_camera_index(0),
	active_model_index(0)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models.size();
}

std::shared_ptr<MeshModel> Scene::GetModel(int index) const
{
	return mesh_models[index];
}

const std::shared_ptr<MeshModel>& Scene::GetActiveModel() const
{
	return mesh_models[active_model_index];
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(int index)
{
	return cameras[index];
}

const Camera& Scene::GetActiveCamera() const
{
	return cameras[active_camera_index];
}

Camera& Scene::GetActiveCamera()
{
	return cameras[active_camera_index];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index;
}

void Scene::SetDrawCamera(bool flag)
{
	this->mode_camera = flag;
}

bool Scene::GetDrawCamera() const
{
	return this->mode_camera;
}

MeshModel& Scene::GetCameraMesh() const
{
	return *this->camera_model;
}

int Scene::getiscolored()
{
	return this->is_colored;
}

void Scene::setiscolored(int colored)
{
	std::cout << endl<<"IN COLOR" << endl;
	this->is_colored = colored;
}

int Scene::GetLightsNumber() const
{
	return this->lights.size();
}

void Scene::AddLight()
{
	this->lights.push_back(std::make_shared<Lights>(1, 1, 1, 1, 1, 1, 1, 1, 1));
}

Lights& Scene::GetLight(int index) const
{
	return*lights[index];
}

int Scene::get_shading_type()
{
	return shading_type;
}

bool Scene::get_reflect_ray() 
{
	return reflect_ray;
}

bool Scene::get_fog()
{
	return fog;
}

bool Scene::get_blur()
{
	return blur;
}

bool Scene::get_msaa()
{
	return msaa;
}

float Scene::get_fog_start()
{
	return fog_start;
}

float Scene::get_fog_end()
{
	return fog_end;
}

void Scene::set_shading_type(int v)
{
	shading_type = v;
}

void Scene::set_reflect_ray(bool v)
{
	reflect_ray = v;
}

void Scene::set_fog(bool v)
{
	fog = v;
}

void Scene::set_blur(bool v)
{
	blur = v;
}

void Scene::set_msaa(bool v)
{
	msaa = v;
}

void Scene::set_fog_start(float v)
{
	fog_start = v;
}

void Scene::set_fog_end(float v)
{
	fog_end = v;
}

int Scene::GetMappingType()
{
	return this->mapping_type;
}

void Scene::SetMappingType(int type)
{
	this->mapping_type = type;
}