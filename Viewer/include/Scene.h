#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "Camera.h"
#include "MeshModel.h"
#include "Lights.h"

using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	std::shared_ptr<MeshModel> GetModel(int index) const;
	const std::shared_ptr<MeshModel>& GetActiveModel() const;
	
	void AddCamera(const Camera& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	
	const Camera& GetActiveCamera() const;
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;
	
	MeshModel& GetCameraMesh() const;
	void SetDrawCamera(bool flag);
	bool GetDrawCamera()const;
	std::shared_ptr<MeshModel>camera_model;

	int getiscolored();
	void setiscolored(int colored);


	int GetLightsNumber()const;
	void AddLight();
	Lights& GetLight(int index)const;
	
	int get_shading_type();
	bool get_reflect_ray();
	bool get_fog();
	bool get_blur();
	bool get_msaa();
	float get_fog_start();
	float get_fog_end();
	void set_shading_type(int v);
	void set_reflect_ray(bool v);
	void set_fog(bool v);
	void set_blur(bool v);
	void set_msaa(bool v);
	void set_fog_start(float v);
	void set_fog_end(float v);

	int GetMappingType();
	void SetMappingType(int type);

private:
	bool mode_camera = false;
	std::vector<std::shared_ptr<MeshModel>> mesh_models;
	std::vector<Camera> cameras;

	int active_camera_index;
	int active_model_index;

	int is_colored = 0;

	int shading_type = 0;
	bool reflect_ray = false;
	bool fog = false;
	bool blur = false;
	bool msaa = false;
	float fog_start = 1;
	float fog_end = -1;
	int lights_num = 0;
	vector<shared_ptr<Lights>> lights;

	int mapping_type = -1;
};