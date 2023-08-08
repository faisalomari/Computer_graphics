#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Face.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
protected:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textureCoords;

	std::vector<Vertex> modelVertices;

	glm::mat4x4 modelTransform;
	glm::mat4x4 worldTransform;

	std::string modelName;

	glm::vec3 color;

	GLuint vbo;
	GLuint vao;

	int texture_mapping = -1;
	glm::mat4x4 model_tranform;
	glm::mat4x4 world_tranform;

	float translate_loc_x = 0;
	float translate_loc_y = 0;
	float translate_loc_z = 0;
	float rotate_loc_x = 0;
	float rotate_loc_y = 0;
	float rotate_loc_z = 0;
	float scale_loc_x = 1;
	float scale_loc_y = 1;
	float scale_loc_z = 1;
	float translate_wor_x = 0;
	float translate_wor_y = 0;
	float translate_wor_z = 0;
	float scale_wor_x = 1;
	float scale_wor_y = 1;
	float scale_wor_z = 1;
	float rotate_wor_x = 0;
	float rotate_wor_y = 0;
	float rotate_wor_z = 0;
	glm::mat4 scaling_mat_loc = glm::mat4(1);
	glm::mat4 translate_mat_loc = glm::mat4(1);
	glm::mat4 rotation_mat_locX = glm::mat4(1);
	glm::mat4 rotation_mat_locY = glm::mat4(1);
	glm::mat4 rotation_mat_locZ = glm::mat4(1);
	glm::mat4 scaling_mat_wor = glm::mat4(1);
	glm::mat4 translate_mat_wor = glm::mat4(1);
	glm::mat4 rotation_mat_worX = glm::mat4(1);
	glm::mat4 rotation_mat_worY = glm::mat4(1);
	glm::mat4 rotation_mat_worZ = glm::mat4(1);
	glm::vec4 center_cordinate;
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName = "");
	virtual ~MeshModel();

	const glm::mat4x4& GetWorldTransformation() const;
	const glm::mat4x4& GetModelTransformation() const;

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	void SetModelTransformation(const glm::mat4x4& modelTransform);

	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3& color);

	const std::string& GetModelName();

	const std::vector<Vertex>& GetModelVertices();

	void TranslateModel(const glm::vec3& translationVector);
	void TranslateWorld(const glm::vec3& translationVector);

	void RotateXModel(double angle);
	void RotateYModel(double angle);
	void RotateZModel(double angle);
	void ScaleXModel(double factor);
	void ScaleYModel(double factor);
	void ScaleZModel(double factor);
	void ScaleModel(double factor);

	void RotateXWorld(double angle);
	void RotateYWorld(double angle);
	void RotateZWorld(double angle);
	void ScaleXWorld(double factor);
	void ScaleYWorld(double factor);
	void ScaleZWorld(double factor);
	void ScaleWorld(double factor);

	GLuint GetVAO() const;

	glm::vec3 ambient_color = glm::vec3(1.0f);
	glm::vec3 specular_color = glm::vec3(1.0f);
	glm::vec3 diffuse_color = glm::vec3(1.0f);

	int GetTextureMappingType();
	void SetTextureMappingType(int type);

	float GetRotateLocx();
	float GetRotateLocy();
	float GetRotateLocz();
	float GetScaleLocx();
	float GetScaleLocy();
	float GetScaleLocz();
	float GetScaleWorx();
	float GetScaleWory();
	float GetScaleWorz();
	float GetTranslateWorx();
	float GetTranslateWory();
	float GetTranslateWorz();
	float GetRotateWorx();
	float GetRotateWory();
	float GetRotateWorz();
	float GetTranslateLocx();
	float GetTranslateLocy();
	float GetTranslateLocz();
	void SetLocalScaleMatrix(float scale_x, float scale_y, float scale_z);
	void SetLocalTranslateMatrix(float translate_x, float  translate_y, float  translate_z);
	void SetLocalRotateMatrix(float rotate_x, float  rotate_y, float  rotate_z);
	void SetWorldScaleMatrix(float scale_x, float scale_y, float scale_z);
	void SetWorldTranslateMatrix(float translate_x, float  translate_y, float  translate_z);
	void SetWorldRotateMatrix(float rotate_x, float  rotate_y, float  rotate_z);
	void UpdateModelTransform();
	glm::vec4 GetCenter();
	int max_z;
	glm::vec3 MeshModel::get_ambient();
	glm::vec3 MeshModel::get_diffuse();
	glm::vec3 MeshModel::get_specular();
};
