#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"


class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces);

	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::string GetFileName(const std::string& filePath);
	static std::string VertcstoString(glm::vec3 vec);
	static bool IsInsideTraingle(const glm::ivec2& curr_point, const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3);
	static float TriangleCalc(int x1, int y1, int x2, int y2, int x3, int y3);
	static glm::vec3 InterpolateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 point, glm::vec3 normal1, glm::vec3 normal2, glm::vec3 normal3, float area1, float area2, float area3);
	static glm::vec3 CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		glm::vec3 edge1 = p2 - p1;
		glm::vec3 edge2 = p3 - p1;
		glm::vec3 face_normal = glm::cross(edge1, edge2);
		face_normal = glm::normalize(face_normal);
		return face_normal;
	}
};