#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Utils.h"

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			textureCoords.push_back(Utils::Vec2fFromStream(issLine));
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	return std::make_shared<MeshModel>(faces, vertices, CalculateNormals(vertices, faces), textureCoords, Utils::GetFileName(filePath));
}
std::vector<glm::vec3> Utils::CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces)
{
	std::vector<glm::vec3> normals(vertices.size());
	std::vector<int> adjacent_faces_count(vertices.size());

	for (int i = 0; i < adjacent_faces_count.size(); i++)
	{
		adjacent_faces_count[i] = 0;
	}

	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);

		int index0 = currentFace.GetVertexIndex(0) - 1;
		int index1 = currentFace.GetVertexIndex(1) - 1;
		int index2 = currentFace.GetVertexIndex(2) - 1;

		glm::vec3 v0 = vertices.at(index0);
		glm::vec3 v1 = vertices.at(index1);
		glm::vec3 v2 = vertices.at(index2);

		glm::vec3 u = v0 - v1;
		glm::vec3 v = v2 - v1;
		glm::vec3 face_normal = glm::normalize(-glm::cross(u, v));

		normals.at(index0) += face_normal;
		normals.at(index1) += face_normal;
		normals.at(index2) += face_normal;

		adjacent_faces_count.at(index0) += 1;
		adjacent_faces_count.at(index1) += 1;
		adjacent_faces_count.at(index2) += 1;
	}

	for (int i = 0; i < normals.size(); i++)
	{
		normals[i] /= adjacent_faces_count[i];
		normals[i] = glm::normalize(normals[i]);
	}

	return normals;
}
std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

std::string Utils::VertcstoString(glm::vec3 vec)
{
	std::string str = " (" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + ")";
	return str;
}

bool Utils::IsInsideTraingle(const glm::ivec2& curr_point, const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3)
{
	float s = (p1.x - p3.x) * (curr_point.y - p3.y) - (p1.y - p3.y) * (curr_point.x - p3.x);
	float t = (p2.x - p1.x) * (curr_point.y - p1.y) - (p2.y - p1.y) * (curr_point.x - p1.x);

	if ((s < 0) != (t < 0) && s != 0 && t != 0)
		return false;

	float k = (p3.x - p2.x) * (curr_point.y - p2.y) - (p3.y - p2.y) * (curr_point.x - p2.x);

	if (k == 0 || (k < 0) == (s + t <= 0))
		return true;

	return false;
}

float Utils::TriangleCalc(int x1, int y1, int x2, int y2, int x3, int y3) {
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

glm::vec3 Utils::InterpolateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 point, glm::vec3 normal1, glm::vec3 normal2, glm::vec3 normal3, float area1, float area2, float area3)
{
	float total_area = area1 + area2 + area3;
	glm::vec3 interpolated_normal = (area1 / total_area) * normal1 + (area2 / total_area) * normal2 + (area3 / total_area) * normal3;
	return interpolated_normal;
}