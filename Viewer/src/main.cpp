#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include "Camera.h"

#include <iostream>

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi=

#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>
using namespace std;
/**
 * Fields
 */
//static int object = 1;
//static int object_tester = 0;
static bool world_or_local = false;
bool show_demo_window = false;
bool show_another_window = false;
char* windowTitle = "OpenGL";
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
static int meshcount = 2;
int windowWidth = 2880;
int windowHeight = 1620;
std::shared_ptr<Scene> scene;
ImGuiIO* imgui;
double zoomFactor = 1;
bool zoomChanged = false;
GLFWwindow* window;
/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus();
ImGuiIO& SetupImgui(GLFWwindow* window);
void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);
float GetAspectRatio();
/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}
/*
int main(int argc, char **argv)
{
	int windowWidth = 3000, windowHeight = 1687.5;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	std::shared_ptr<MeshModel> Camera_Mesh = Utils::LoadMeshModel("..\\Data\\Camera.obj");
	std::shared_ptr<Camera> camera(new Camera());
	scene.AddCamera(camera);
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}
*/

void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	scene->GetActiveCamera().SetAspectRatio(GetAspectRatio());
}

ImGuiIO& SetupImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	glfwSetScrollCallback(window, glfw_OnMouseScroll);

	return io;
}

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}

bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}

	imgui = &SetupImgui(window);

	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}

float GetAspectRatio()
{
	return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void HandleImguiInput()
{
	if (!imgui->WantCaptureKeyboard)
	{
		if (imgui->KeysDown[49]) // 1
		{
			scene->GetActiveModel()->RotateXModel(M_PI / 200);
		}

		if (imgui->KeysDown[50]) // 2
		{
			scene->GetActiveModel()->RotateXModel(-M_PI / 200);
		}

		if (imgui->KeysDown[51]) // 3
		{
			scene->GetActiveModel()->RotateYModel(M_PI / 200);
		}

		if (imgui->KeysDown[52]) // 4
		{
			scene->GetActiveModel()->RotateYModel(-M_PI / 200);
		}

		if (imgui->KeysDown[53]) // 5
		{
			scene->GetActiveModel()->RotateZModel(M_PI / 200);
		}

		if (imgui->KeysDown[54]) // 6
		{
			scene->GetActiveModel()->RotateZModel(-M_PI / 200);
		}

		if (imgui->KeysDown[45]) // -
		{
			scene->GetActiveModel()->ScaleModel(1 / 1.01);
		}

		if (imgui->KeysDown[61]) // +
		{
			scene->GetActiveModel()->ScaleModel(1.01);
		}

		if (imgui->KeysDown[65]) // a
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(-0.02, 0, 0));
		}

		if (imgui->KeysDown[68]) // d
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0.02, 0, 0));
		}

		if (imgui->KeysDown[83]) // s
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, 0.02));
		}

		if (imgui->KeysDown[87]) // w
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, -0.02));
		}
	}

}

int main(int argc, char** argv)
{

	//std::cout << "HERE1 " << endl;
	if (!Setup(windowWidth, windowHeight, windowTitle))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}

	//std::cout << "HERE2 " << endl;
	scene = std::make_shared<Scene>();	glm::vec3 eye = glm::vec3(0, 0, 10);
	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	scene->AddCamera(Camera(eye, at, up, GetAspectRatio()));
	//std::cout << "HERE3 " << endl;
	/*scene->AddLight(std::make_shared<PointLight>(glm::vec3(0, 0, 15), glm::vec3(1, 1, 1)));
	scene->AddLight(std::make_shared<PointLight>(glm::vec3(0, 5, 5), glm::vec3(0, 0, 0)));
	scene->AddLight(std::make_shared<PointLight>(glm::vec3(-5, 0, 0), glm::vec3(0, 0, 0)));*/

	Renderer renderer;
	renderer.LoadShaders();
	renderer.LoadTextures();

	while (!glfwWindowShouldClose(window))
	{
		// Poll and process events
		glfwPollEvents();
		// Imgui stuff
		//start frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//-------------------
		DrawImguiMenus();
		ImGui::Render();
		HandleImguiInput();
		//std::cout << "HERE43 " << endl;
		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//std::cout << "HERE44 " << endl;
		// Render scene
		renderer.Render(scene);
		//std::cout << "HERE45 " << endl;
		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//std::cout << "HERE46 " << endl;
		// Swap front and back buffers
		glfwSwapBuffers(window);
	}
	//std::cout << "HERE5 " << endl;
	glfwTerminate();
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{

	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
/*
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
		Renderer r = Renderer(frameBufferWidth, frameBufferHeight);
		glfwSetWindowAspectRatio(window, r.GetViewportWidth(), r.GetViewportHeight());
	}
	

	if (!io.WantCaptureKeyboard)
	{
		if (scene.GetModelCount() > 0) {
			scene.GetActiveModel().Set_LorW(world_or_local);
		}
		// TODO: Handle keyboard events here
		for (int i = 48; i <= 57; i++) {// to move from one object to another to control in the local
			if (io.KeysDown[i]) { //according to the number 1-9 (in the AscII from 49-57)
				if (scene.GetModelCount() >= 2)
				{
					object_tester = i - 48;
					if ((object_tester < 1 )|| (object_tester > scene.GetModelCount())){// if it isn't in the numbers range
						std::cout <<'\n' << "Model Number " << object_tester << " out of range";
					}
					else {
						object = object_tester;
					}
					
				}
			}
		}
		if (io.KeysDown[32])//to replace the controls between world and local
		{
			if (scene.GetModelCount() > 0)
			{
				world_or_local = !world_or_local;
				scene.GetActiveModel().Set_LorW(world_or_local);
			}
		}

		if (io.KeysDown[65])//A to translet left
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_translate_mat(curr_mesh.Get_localX_translate(), curr_mesh.Get_localY_translate(), curr_mesh.Get_localZ_translate() - 1);
				}
				else
				{
					scene.GetActiveModel().Set_World_translate_mat(curr_mesh.Get_worldX_translate(), curr_mesh.Get_worldY_translate(), curr_mesh.Get_worldZ_translate() - 1);
				}
			}
		}
		if (io.KeysDown[68])//D key to translate right
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_translate_mat(curr_mesh.Get_localX_translate(), curr_mesh.Get_localY_translate(), curr_mesh.Get_localZ_translate()+1);
				}
				else
				{
					scene.GetActiveModel().Set_World_translate_mat(curr_mesh.Get_worldX_translate(), curr_mesh.Get_worldY_translate(), curr_mesh.Get_worldZ_translate() + 1);
				}
			}
		}
		if (io.KeysDown[87])//W key to translate up
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_translate_mat(curr_mesh.Get_localX_translate(), curr_mesh.Get_localY_translate() + 15, curr_mesh.Get_localZ_translate());
				}
				else
				{
					scene.GetActiveModel().Set_World_translate_mat(curr_mesh.Get_worldX_translate(), curr_mesh.Get_worldY_translate() + 15, curr_mesh.Get_worldZ_translate());
				}
			}
		}
		if (io.KeysDown[83])//S key to translate down
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_translate_mat(curr_mesh.Get_localX_translate(), curr_mesh.Get_localY_translate() - 15, curr_mesh.Get_localZ_translate());
				}
				else
				{
					scene.GetActiveModel().Set_World_translate_mat(curr_mesh.Get_worldX_translate(), curr_mesh.Get_worldY_translate() - 15, curr_mesh.Get_worldZ_translate());
				}
			}
		}
		if (io.KeysDown[69])//E key to scale up
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_scale_mat(curr_mesh.Get_localX_scale() + 1, curr_mesh.Get_localY_scale() + 1, curr_mesh.Get_localZ_scale() + 1);
				}
				else
				{
					scene.GetActiveModel().Set_World_scale_mat(curr_mesh.Get_worldX_scale() + 1, curr_mesh.Get_worldY_scale() + 1, curr_mesh.Get_worldZ_scale() + 1);
				}
			}
		}
		if (io.KeysDown[81])//Q key to scale down
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_scale_mat(curr_mesh.Get_localX_scale() - 1, curr_mesh.Get_localY_scale() - 1, curr_mesh.Get_localZ_scale() - 1);
				}
				else
				{
					scene.GetActiveModel().Set_World_scale_mat(curr_mesh.Get_worldX_scale() - 1, curr_mesh.Get_worldY_scale() - 1, curr_mesh.Get_worldZ_scale() - 1);
				}
			}
		}
		if (io.KeysDown[88])//Z key to rotate++ of Y-axis
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_rotation_mat(curr_mesh.Get_localX_rotation() + 1, curr_mesh.Get_localY_rotation(), curr_mesh.Get_localZ_rotation());
				}
				else
				{
					scene.GetActiveModel().Set_World_rotation_mat(curr_mesh.Get_worldX_rotation() + 1, curr_mesh.Get_worldY_rotation(), curr_mesh.Get_worldZ_rotation());
				}
			}
		}
		if (io.KeysDown[90])//X key to rotate -- of Y-axis
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_rotation_mat(curr_mesh.Get_localX_rotation() - 1, curr_mesh.Get_localY_rotation(), curr_mesh.Get_localZ_rotation());
				}
				else
				{
					scene.GetActiveModel().Set_World_rotation_mat(curr_mesh.Get_worldX_rotation() - 1, curr_mesh.Get_worldY_rotation(), curr_mesh.Get_worldZ_rotation());
				}
			}
		}
		if (io.KeysDown[82])//R key the same as Y but for Z-axis
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_rotation_mat(curr_mesh.Get_localX_rotation(), curr_mesh.Get_localY_rotation() + 1, curr_mesh.Get_localZ_rotation());
				}
				else
				{
					scene.GetActiveModel().Set_World_rotation_mat(curr_mesh.Get_worldX_rotation(), curr_mesh.Get_worldY_rotation() + 1, curr_mesh.Get_worldZ_rotation());
				}
			}
		}
		if (io.KeysDown[84])//T key the same as Y but for Z axis
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_rotation_mat(curr_mesh.Get_localX_rotation(), curr_mesh.Get_localY_rotation() - 1, curr_mesh.Get_localZ_rotation());
				}
				else
				{
					scene.GetActiveModel().Set_World_rotation_mat(curr_mesh.Get_worldX_rotation(), curr_mesh.Get_worldY_rotation() - 1, curr_mesh.Get_worldZ_rotation());
				}
			}
		}
		if (io.KeysDown[70])//F key the same as Z but for x-axis
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_rotation_mat(curr_mesh.Get_localX_rotation(), curr_mesh.Get_localY_rotation(), curr_mesh.Get_localZ_rotation() + 1);
				}
				else
				{
					scene.GetActiveModel().Set_World_rotation_mat(curr_mesh.Get_worldX_rotation(), curr_mesh.Get_worldY_rotation(), curr_mesh.Get_worldZ_rotation() + 1);
				}
			}
		}
		if (io.KeysDown[71])//G key the same as X but for the x-axis
		{
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_rotation_mat(curr_mesh.Get_localX_rotation(), curr_mesh.Get_localY_rotation(), curr_mesh.Get_localZ_rotation() - 1);
				}
				else
				{
					scene.GetActiveModel().Set_World_rotation_mat(curr_mesh.Get_worldX_rotation(), curr_mesh.Get_worldY_rotation(), curr_mesh.Get_worldZ_rotation() - 1);
				}
			}
		}
	}

	if (!io.WantCaptureMouse)// no mouse events
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}*/

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus()
{/*
	if (scene.GetModelCount() > 0) {
		scene.GetActiveModel().Set_LorW(world_or_local);
	}*/
	/**
 * MeshViewer menu
 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene->AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);

				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (world_or_local) {
		ImGui::Text("The controls are on World");
	}
	else {
		ImGui::Text("The controls are on Local");
	}

	// TODO: Add more controls as needed

	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{ //setting variables to use
		static float f = 0.0f;
		static int counter = 0;
		static int scale_v = 1;
		static bool mesh = TRUE;
		static float w_scale_x = 0.0f;
		static float w_scale_y = 0.0f;
		static float w_scale_z = 0.0f;
		static float w_translation_x = 0.0f;
		static float w_translation_y = 0.0f;
		static float w_translation_z = 0.0f;
		static float w_rotate_x = 0.0f;
		static float w_rotate_y = 0.0f;
		static float w_rotate_z = 0.0f;
		/*
		if (scene.GetModelCount() >= 1) {//if there is a model, adding the variables that will be used in the windows
			MeshModel curr_mesh = scene.GetActiveModel();
			l_translation_x = curr_mesh.Get_localX_translate();
			l_translation_y = curr_mesh.Get_localY_translate();
			l_translation_z = curr_mesh.Get_localZ_translate();
			l_rotate_x = curr_mesh.Get_localX_rotation();
			l_rotate_y = curr_mesh.Get_localY_rotation();
			l_rotate_z = curr_mesh.Get_localZ_rotation();
			l_scale_x = curr_mesh.Get_localX_scale();
			l_scale_y = curr_mesh.Get_localY_scale();
			l_scale_z = curr_mesh.Get_localZ_scale();

			w_scale_x = curr_mesh.Get_worldX_scale();
			w_scale_y = curr_mesh.Get_worldY_scale();
			w_scale_z = curr_mesh.Get_worldZ_scale();
			w_rotate_x = curr_mesh.Get_worldX_rotation();
			w_rotate_y = curr_mesh.Get_worldY_rotation();
			w_rotate_z = curr_mesh.Get_worldZ_rotation();
			w_translation_x = curr_mesh.Get_worldX_translate();
			w_translation_y = curr_mesh.Get_worldY_translate();
			w_translation_z = curr_mesh.Get_worldZ_translate();

		}
		*/
		if (scene->GetModelCount() > 0)
		{
			ImGui::ColorEdit3("ambient color", (float*)&scene->GetActiveModel()->ambient_color);
			ImGui::ColorEdit3("diffuse color", (float*)&scene->GetActiveModel()->diffuse_color);
			ImGui::ColorEdit3("specular color", (float*)&scene->GetActiveModel()->specular_color);
			static float translatex = 0;
			static float scalex = 1;
			static float rotatex = 0;
			static float translatey = 0;
			static float scaley = 1;
			static float rotatey = 0;
			static float translatez = 0;
			static float scalez = 1;
			static float rotatez = 0;
			static bool world = true;
			static int texture_mapping = scene->GetActiveModel()->GetTextureMappingType();
			static int scene_mapping = scene->GetMappingType();
			const char** items;
			std::vector<std::string> modelStrings;
			items = new const char*[scene->GetModelCount()];
			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				std::ostringstream s;
				s << scene->GetModel(i)->GetModelName();
				std::string mystring = s.str();
				modelStrings.push_back(mystring);
			}
			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				items[i] = modelStrings[i].c_str();
			}
			int currentModelIndex = scene->GetActiveModelIndex();
			//ImGui::Combo("Active Model", &currentModelIndex, items, scene->GetModelCount());

			if (currentModelIndex != scene->GetActiveModelIndex())
			{
				scene->SetActiveModelIndex(currentModelIndex);
			}
			if (scene->GetModelCount())
			{
				if (ImGui::RadioButton("none", &texture_mapping, -1))
				{
					scene->GetActiveModel()->SetTextureMappingType(texture_mapping);
				}
				if (ImGui::RadioButton("Planer", &texture_mapping, 0))
				{
					scene->GetActiveModel()->SetTextureMappingType(texture_mapping);
				}

				if (ImGui::RadioButton("None", &scene_mapping, -1))
				{
					scene->SetMappingType(scene_mapping);
				}
				if (ImGui::RadioButton("Normal Mapping", &scene_mapping, 1))
				{
					scene->SetMappingType(scene_mapping);
				}

			}
			delete items;
			translatex = scene->GetActiveModel()->GetTranslateWorx();
			translatey = scene->GetActiveModel()->GetTranslateWory();
			translatez = scene->GetActiveModel()->GetTranslateWorz();
			scalex = scene->GetActiveModel()->GetScaleWorx();
			scaley = scene->GetActiveModel()->GetScaleWory();
			scalez = scene->GetActiveModel()->GetScaleWorz();
			rotatex = scene->GetActiveModel()->GetRotateWorx();
			rotatey = scene->GetActiveModel()->GetRotateWory();
			rotatez = scene->GetActiveModel()->GetRotateWorz();
			if (ImGui::InputFloat("translation x", &translatex, 0.1f, 1.0f, "%.5f"))
			{
				scene->GetActiveModel()->SetWorldTranslateMatrix(translatex, translatey, translatez);

			}
			if (ImGui::InputFloat("translation y", &translatey, 0.1f, 1.0f, "%.5f"))
			{
				scene->GetActiveModel()->SetWorldTranslateMatrix(translatex, translatey, translatez);
			}
			if (ImGui::InputFloat("translation z", &translatez, 0.1f, 1.0f, "%.53f"))
			{
				scene->GetActiveModel()->SetWorldTranslateMatrix(translatex, translatey, translatez);
			}
			if (ImGui::InputFloat("rotation x ", &rotatex, 1.0f, 10.0f, "%.5f"))
			{
				scene->GetActiveModel()->SetWorldRotateMatrix(rotatex, rotatey, rotatez);
			}
			if (ImGui::InputFloat("rotation x ", &rotatey, 1.0f, 10.0f, "%.5f"))
			{
				scene->GetActiveModel()->SetWorldRotateMatrix(rotatex, rotatey, rotatez);
			}
			if (ImGui::InputFloat("rotation x", &rotatez, 1.0f, 10.0f, "%.5f"))
			{
				scene->GetActiveModel()->SetWorldRotateMatrix(rotatex, rotatey, rotatez);
			}
		}
		/*
		ImGui::Begin("World's Controls");//creating World's controls window
		if (scene->GetModelCount())
		{
			if (ImGui::SliderFloat("scale x", &w_scale_x, 0, 1000))
			{
				if (scene->GetModelCount() >= 1)
				{
					scene->GetActiveModel().Set_World_scale_mat(w_scale_x, w_scale_y, w_scale_z);
				}
			}
			if (ImGui::SliderFloat("scale y", &w_scale_y, 0, 1000))
			{
				if (scene->GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_scale_mat(w_scale_x, w_scale_y, w_scale_z);
				}

			}
			if (ImGui::SliderFloat("scale z", &w_scale_z, 0, 1000))
			{
				if (scene.GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_scale_mat(w_scale_x, w_scale_y, w_scale_z);
				}
			}

			//translation sliders
			if (ImGui::SliderFloat("translate x", &w_translation_x, -640, 640))
			{
				if (scene.GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_translate_mat(w_translation_x, w_translation_y, w_translation_z);
				}

			}
			if (ImGui::SliderFloat("translate y", &w_translation_y, -320, 320))
			{
				if (scene.GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_translate_mat(w_translation_x, w_translation_y, w_translation_z);
				}

			}
			if (ImGui::SliderFloat("translate z", &w_translation_z, 0, 1000))
			{
				if (scene.GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_translate_mat(w_translation_x, w_translation_y, w_translation_z);
				}
			}

			//rotation sliders
			if (ImGui::SliderFloat("rotate x", &w_rotate_x, -360, 360))
			{
				if (scene.GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_rotation_mat(w_rotate_x, w_rotate_y, w_rotate_z);
				}

			}
			if (ImGui::SliderFloat("rotate y", &w_rotate_y, -360, 360))//
			{
				if (scene.GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_rotation_mat(w_rotate_x, w_rotate_y, w_rotate_z);
				}

			}
			if (ImGui::SliderFloat("rotate z", &w_rotate_z, -360, 360))//
			{
				if (scene.GetModelCount() != 0)
				{
					scene.GetActiveModel().Set_World_rotation_mat(w_rotate_x, w_rotate_y, w_rotate_z);
				}
			}
		}
		ImGui::End();
		*/
		//Local controls
		/*
		ImGui::Begin("Local Controls");
		if (scene.GetModelCount())
		{
			{//model selction
				if (scene.GetModelCount() >= 2)
				{
					ImGui::SliderInt("Model Selction", &object, 1, scene.GetModelCount());
					scene.SetActiveModelIndex(object - 1);
				}

			}

			{//transformation selection
				//scaling sliders
				if (ImGui::SliderFloat("scale x", &l_scale_x, 0, 1000))
				{
					if (scene.GetModelCount() >= 1)
					{
						scene.GetActiveModel().Set_Local_scale_mat(l_scale_x, l_scale_y, l_scale_z);
					}

				}
				if (ImGui::SliderFloat("scale y", &l_scale_y, 0, 1000))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_scale_mat(l_scale_x, l_scale_y, l_scale_z);
					}

				}
				if (ImGui::SliderFloat("scale z", &l_scale_z, 0, 1000))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_scale_mat(l_scale_x, l_scale_y, l_scale_z);
					}
				}

				//translation sliders
				if (ImGui::SliderFloat("translate x", &l_translation_x, -640, 640))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_translate_mat(l_translation_x, l_translation_y, l_translation_z);
					}

				}
				if (ImGui::SliderFloat("translate y", &l_translation_y, -320, 320))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_translate_mat(l_translation_x, l_translation_y, l_translation_z);
					}

				}
				if (ImGui::SliderFloat("translate z", &l_translation_z, 0, 1000))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_translate_mat(l_translation_x, l_translation_y, l_translation_z);
					}
				}

				//rotation sliders
				if (ImGui::SliderFloat("rotate x", &l_rotate_x, -360, 360))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_rotation_mat(l_rotate_x, l_rotate_y, l_rotate_z);
					}

				}
				if (ImGui::SliderFloat("rotate y", &l_rotate_y, -360, 360))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_rotation_mat(l_rotate_x, l_rotate_y, l_rotate_z);
					}

				}
				if (ImGui::SliderFloat("rotate z", &l_rotate_z, -360, 360))
				{
					if (scene.GetModelCount() != 0)
					{
						scene.GetActiveModel().Set_Local_rotation_mat(l_rotate_x, l_rotate_y, l_rotate_z);
					}
				}


			}
			ImGui::ColorEdit3("ambient", (float*)&scene.GetActiveModel().ambient_color);
			ImGui::ColorEdit3("diffuse", (float*)&scene.GetActiveModel().diffuse_color);
			ImGui::ColorEdit3("specular", (float*)&scene.GetActiveModel().specular_color);
		}
		ImGui::End();
		*/
	}

	// 3. Show another simple window.
	/*
	{
		int camera_num = 0;
		int item_num = 0;
		float fovy = 0.25;
		float asp = 1;
		bool draw_camera = false;
		float down = scene.GetActiveCamera().GetDown();
		float top = scene.GetActiveCamera().GetTop();
		float near_c = scene.GetActiveCamera().GetNear1();
		float far_c = scene.GetActiveCamera().GetFar1();
		float right = scene.GetActiveCamera().GetRight();
		float left = scene.GetActiveCamera().GetLeft();
		//scene.SetActiveCameraIndex(0);
		int ortho_pers_flag = 1;
		if (scene.GetCameraCount() >= 1) {
			ImGui::Begin("Camera Control Window");
			static int camera_index = 1;
			if (scene.GetCameraCount() > 1)
			{
				static int sliderint = 1;
				ImGui::SliderInt("Camera Selction", &camera_index, 1, scene.GetCameraCount());
				scene.SetActiveCameraIndex(camera_index - 1);
			}
			//if (ImGui::Button("Add Camera"))
			//{
			//	scene.AddCamera(std::make_shared<Camera>());
			//}
			{
				ImGui::RadioButton("Press for perspective", &ortho_pers_flag, 0);
				ImGui::SameLine();
				ImGui::RadioButton("orthotographic", &ortho_pers_flag, 1);
			}
			scene.GetActiveCamera().Set_flag_Orth_pro(ortho_pers_flag);
			if (scene.GetCameraCount())
			{
				if (!ortho_pers_flag)
				{
					scene.GetActiveCamera().set_perspective(glm::radians(fovy));
					if (ImGui::SliderFloat("fovy", &fovy, -200, 200))
					{
						scene.GetActiveCamera().set_perspective(glm::radians(fovy));
					}
					if (ImGui::SliderFloat("Far", &far_c, -10, 10))
					{
						scene.GetActiveCamera().SetFar1(far_c);
					}
					if (ImGui::SliderFloat("Near", &near_c, -10, 10))
					{
						scene.GetActiveCamera().SetNear1(near_c);
					}
					if (ImGui::SliderFloat("Aspect", &asp, 0, 1))
					{
						scene.GetActiveCamera().SetAsp(asp);
					}
				}
				else
				{
					if (ImGui::SliderFloat("Near", &near_c, -100, 100))
					{
						scene.GetActiveCamera().SetNear1(near_c);
					}
					if (ImGui::SliderFloat("Far", &far_c, -100, 100))
					{
						scene.GetActiveCamera().SetFar1(far_c);

					}
					if (ImGui::SliderFloat("Left", &left, -100, 100))
					{
						scene.GetActiveCamera().SetLeft(left);

					}
					if (ImGui::SliderFloat("Right", &right, -100, 100))
					{
						scene.GetActiveCamera().SetRight(right);

					}
					if (ImGui::SliderFloat("Top", &top, -100, 100))
					{
						scene.GetActiveCamera().SetTop(top);

					}
					if (ImGui::SliderFloat("Bottom", &down, -100, 100))
					{
						scene.GetActiveCamera().SetDown(down);
					}

				}

			}

			float cam_tr_xl = scene.GetActiveCamera().Get_Local_transX();
			float cam_rot_xl = scene.GetActiveCamera().Get_Local_rotX();
			float cam_tr_yl = scene.GetActiveCamera().Get_Local_transY();
			float cam_rot_yl = scene.GetActiveCamera().Get_Local_rotY();
			float cam_tr_zl = scene.GetActiveCamera().Get_Local_transZ();
			float cam_rot_zl = scene.GetActiveCamera().Get_Local_rotZ();
			float cam_tr_xw = scene.GetActiveCamera().Get_World_transX();
			float cam_rot_xw = scene.GetActiveCamera().Get_World_rotX();
			float cam_tr_yw = scene.GetActiveCamera().Get_World_transY();
			float cam_rot_yw = scene.GetActiveCamera().Get_World_rotY();
			float cam_tr_zw = scene.GetActiveCamera().Get_World_transZ();
			float cam_rot_zw = scene.GetActiveCamera().Get_World_rotZ();
			bool axes;
			bool box;
			bool faces;
			bool vertices;
			if (scene.GetModelCount()) {
				axes = scene.GetActiveModel().Get_axes_flag();
				box = scene.GetActiveModel().Get_box_flag();
				faces = scene.GetActiveModel().Get_faces_flag();
				vertices = scene.GetActiveModel().Get_vertices_flag();
			}


			static bool trans;
			static bool colored = false;
			if (ImGui::SliderFloat("Local translate x", &cam_tr_xl, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetLocalTranslateMatrix(cam_tr_xl, cam_tr_yl, cam_tr_zl);
				}
			}
			if (ImGui::SliderFloat("Local translate y", &cam_tr_yl, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetLocalTranslateMatrix(cam_tr_xl, cam_tr_yl, cam_tr_zl);
				}
			}
			if (ImGui::SliderFloat("Local translate z", &cam_tr_zl, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetLocalTranslateMatrix(cam_tr_xl, cam_tr_yl, cam_tr_zl);
				}
			}

			if (ImGui::SliderFloat("World translate x", &cam_tr_xw, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetWorldTranslateMatrix(cam_tr_xw, cam_tr_yw, cam_tr_zw);
				}
			}
			if (ImGui::SliderFloat("World translate y", &cam_tr_yw, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetWorldTranslateMatrix(cam_tr_xw, cam_tr_yw, cam_tr_zw);
				}
			}
			if (ImGui::SliderFloat("World translate z", &cam_tr_zw, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetWorldTranslateMatrix(cam_tr_xw, cam_tr_yw, cam_tr_zw);
				}
			}
			if (ImGui::SliderFloat("Local rotate x", &cam_rot_xl, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetLocalRotateMatrix(cam_rot_xl, cam_rot_yl, cam_rot_zl);
				}
			}
			if (ImGui::SliderFloat("Local rotate y", &cam_rot_yl, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetLocalRotateMatrix(cam_rot_xl, cam_rot_yl, cam_rot_zl);
				}
			}
			if (ImGui::SliderFloat("Local rotate z", &cam_rot_zl, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetLocalRotateMatrix(cam_rot_xl, cam_rot_yl, cam_rot_zl);
				}
			}

			if (ImGui::SliderFloat("World rotate x", &cam_rot_xw, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetWorldRotateMatrix(cam_rot_xw, cam_rot_yw, cam_rot_zw);
				}
			}
			if (ImGui::SliderFloat("World rotate y", &cam_rot_yw, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetWorldRotateMatrix(cam_rot_xw, cam_rot_yw, cam_rot_zw);
				}
			}
			if (ImGui::SliderFloat("World rotate z", &cam_rot_zw, -640, 640))//
			{
				if (scene.GetCameraCount() != 0)
				{
					scene.GetActiveCamera().SetWorldRotateMatrix(cam_rot_xw, cam_rot_yw, cam_rot_zw);
				}
			}

			if (scene.GetModelCount()) {
				ImGui::Checkbox("Triangle Bounding Box New", &scene.GetActiveModel().GetTrianglebox());
				if (ImGui::Checkbox("Axes flag", &axes))
				{
					scene.GetActiveModel().Set_axes_flag(!scene.GetActiveModel().Get_axes_flag());
				}

				if (ImGui::Checkbox("Bounding box flag", &box))
				{
					scene.GetActiveModel().Set_box_flag(!scene.GetActiveModel().Get_box_flag());
				}

				if (ImGui::Checkbox("Faces flag", &faces))
				{
					scene.GetActiveModel().Set_faces_flag(!scene.GetActiveModel().Get_faces_flag());
				}

				if (ImGui::Checkbox("Vertices flag", &vertices))
				{
					scene.GetActiveModel().Set_vertices_flag(!scene.GetActiveModel().Get_vertices_flag());
				}
				if (ImGui::Checkbox("Coloring", &colored))
				{
					if (colored)
					{
						scene.setiscolored(1);
					}
					else
					{
						scene.setiscolored(0);
					}
				}
			}
			ImGui::End();
		}
	}
	*/

	ImGui::Begin("lights Control Window");
	if (ImGui::Button("Add light"))
	{
		scene->AddLight();
	}
	static int Light_index = 1;
	if (scene->GetLightsNumber() > 1)
	{
		static int sliderint = 1;
		ImGui::SliderInt("Light Selction", &Light_index, 1, scene->GetLightsNumber());
	}
	if (scene->GetLightsNumber() >= 1)
	{
		int shading_type = scene->get_shading_type();
		bool reflect_ray = scene->get_reflect_ray();
		bool fog = scene->get_fog();
		bool blur = scene->get_blur();
		bool msaa = scene->get_msaa();
		float fog_start = scene->get_fog_start();
		float fog_end = scene->get_fog_end();
		Lights light = scene->GetLight(Light_index - 1);

		static bool flag = false;

		ImGui::ColorEdit3("ambient", (float*)&scene->GetLight(Light_index - 1).ambient_color);
		ImGui::ColorEdit3("diffuse", (float*)&scene->GetLight(Light_index - 1).diffuse_color);
		ImGui::ColorEdit3("specular", (float*)&scene->GetLight(Light_index - 1).specular_color);

		//ImGui::RadioButton("flat", &shading_type, 0);
		//ImGui::SameLine();
		//ImGui::RadioButton("Phong", &shading_type, 1);

		ImGui::InputFloat("move x", (float*)&scene->GetLight(Light_index - 1).location.x, 1.0f, 1.0f, "%.3f");
		ImGui::InputFloat("move y", (float*)&scene->GetLight(Light_index - 1).location.y, 1.0f, 1.0f, "%.3f");
		ImGui::InputFloat("move z", (float*)&scene->GetLight(Light_index - 1).location.z, 1.0f, 1.0f, "%.3f");
		ImGui::InputFloat("Alpha", (float*)&scene->GetLight(Light_index - 1).alpha, 1.0f, 1.0f, "%.3f");
		ImGui::Checkbox("Reflected rays", &reflect_ray);
		ImGui::Checkbox("Fog", &fog);
		ImGui::InputFloat("fog start", (float*)&fog_start, 1.0f, 1.0f, "%.3f");
		ImGui::InputFloat("fog end", (float*)&fog_end, 1.0f, 1.0f, "%.3f");

		ImGui::Checkbox("Blur", &blur);

	    scene->set_shading_type(shading_type);
		scene->set_reflect_ray(reflect_ray);
		scene->set_fog(fog);
		scene->set_blur(blur);
		scene->set_fog_start(fog_start);
		scene->set_fog_end(fog_end);

	}
	ImGui::End();




	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}