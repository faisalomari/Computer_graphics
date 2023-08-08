#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>
#include <Utils.h>
#include "Texture2D.h"

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	Renderer();
	virtual ~Renderer();
	void Draw_drawingANDcircle();
	/*old renderer head
	void Render(const Scene& scene);
	*/
	void Render(const std::shared_ptr<Scene>& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void Renderer::DrawMesh(MeshModel mesh, std::vector<Face> faces, std::vector<glm::vec3> vertices, Scene scene, int width, int height, int index);
	void sc_func(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int min_x, int max_x, int min_y, int max_y, glm::vec3 color);
	void Phong(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int minx, int maxx, int miny, int maxy, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3, Scene scene, MeshModel mesh);
	void DrawLight(int centerx, int centery);
	glm::vec3 CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 getColor(glm::vec3 point, glm::vec3 normal, Scene& scene, MeshModel& mesh);

	void LoadShaders();
	void LoadTextures();
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void axes_function(int half_width,int half_height,Scene scene);
	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;

	float* color_buffer;
	float* Z_buffer;
	vector<glm::vec3> temp_color_buffer;
	vector<float> z_buffer;
	float is_orth = 1;
	float max_z = -1 * INFINITY;
	float min_z = INFINITY;

	ShaderProgram lightShader;
	ShaderProgram colorShader;
	ShaderProgram shader;
	ShaderProgram skyboxShader;
	GLuint cubeMapTexture;
	GLuint cubeVAO, cubeVBO;
	GLuint skyboxVAO, skyboxVBO;
	Texture2D texture1;
	Texture2D texture2;
	Texture2D texture3;
	int lightcount;
};
