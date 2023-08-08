#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Renderer.h"
#include "InitShader.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer()
{

}

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
	/*
	for (int i = 0; i < viewport_width * viewport_height; i++)
	{
		z_buffer.push_back(INFINITY);
	}
	for (int i = 0; i < viewport_width * viewport_height; i++)
	{
		temp_color_buffer.push_back(glm::vec3(0.4, 0.4, 1));
	}*/
}

Renderer::~Renderer()
{
	delete[] color_buffer;
	delete[] Z_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;
	
	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int x0 = p1[0];
	int y0 = p1[1];
	int x1 = p2[0];
	int y1 = p2[1];
	if (x0 > x1) {
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	int x, y,l=1;
	float a, e, c;
	float diff_x = x1 - x0, diff_y = y1 - y0;
	a = diff_y / diff_x;
	c = y0 - a * x0;
	if (a>=-1 && a<=1) {
		x = x0;
		y = y0;
		if (diff_y < 0) {
			diff_y = -diff_y;
			l = -1;
		}
		e = (2 * diff_y) - diff_x;
		while (x <= x1) {
			PutPixel(x, y, color);
			if (e > 0) {
				y = y + l;
				e = e - 2 * diff_x;
			}
			x++;
			e = e + 2 * diff_y;
		}
	}
	else if (a > 1) {
		x = x0;
		y = y0;
		e = (2 * diff_x) - diff_y;
		while (y <= y1) {
			PutPixel(x, y, color);

			if (e > 0) {
				x++;
				e = e - 2 *  diff_y;
			}
			y++;
			e = e + 2 * diff_x;
		}
	}
	else if (a < -1) {
		x = x0;
		y = y0;
		diff_y = -diff_y;
		e = (2 * diff_x) - diff_y;
		while (y >= y1) {
			PutPixel(x, y, color);

			if (e > 0) {
				x++;
				e = e - 2 * diff_y;
			}
			y--;
			e = e + 2 * diff_x;
		}
	}

}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
	Z_buffer = new float[w * h];
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	//replace this to 36 oe 0 
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Draw_drawingANDcircle()
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	// draw circle
	const glm::ivec2 center(half_width, half_height);
	const glm::vec3 col(250.f, 0.f, 250.f);
	const glm::vec3 col2(0.f, 0.75, 0.5);
	const glm::vec3 col3(1.f, 0, 0.5);
	int  l = 360;
	float x, y, degree, step = 0.001, sinn, coss, i;
	const int radius = 100;
	//circle circumference
	for (i = 0; i <= 360; i += step) {
		degree = (i * M_PI) / 180;
		sinn = sin(degree);
		coss = cos(degree);
		x = half_width + radius * (sinn);
		y = half_height + radius * (coss);
		x = round(x);
		y = round(y);
		PutPixel(x, y, col);
	}
	//circle lines from center to circumference
	step = 45;
	for (i = 0; i <= 360; i += step) {
		degree = (i * M_PI) / 180;
		sinn = sin(degree);
		coss = cos(degree);
		x = half_width + radius * (sinn);
		y = half_height + radius * (coss);
		x = round(x);
		y = round(y);
		glm::ivec2 p(x, y);
		DrawLine(center, p, col2);
	}
	//The draw of mine
	const glm::ivec2 pp1(150, 186);
	const glm::ivec2 pp2(240, 186);
	const glm::ivec2 pp3(270, 125);
	const glm::ivec2 pp4(350, 100);
	const glm::ivec2 pp5(500, 156);
	const glm::ivec2 pp6(450, 250);
	const glm::ivec2 pp7(355, 290);
	const glm::ivec2 pp8(342, 290);
	const glm::ivec2 pp9(335, 290);
	const glm::ivec2 pp10(325, 290);
	const glm::ivec2 pp11(500, 350);
	const glm::ivec2 pp12(260, 230);
	const glm::ivec2 pp13(185, 195);
	const glm::ivec2 pp14(500, 430);
	const glm::ivec2 pp15(470, 410);
	const glm::ivec2 pp16(364, 600);
	const glm::ivec2 pp17(260, 600);
	const glm::ivec2 pp18(364, 550);
	const glm::ivec2 pp19(270, 550);
	const glm::ivec2 pp20(355, 380);
	const glm::ivec2 pp21(170, 530);
	const glm::ivec2 pp22(185, 500);
	const glm::ivec2 pp23(160, 430);
	const glm::ivec2 pp24(120, 410);
	const glm::ivec2 pp25(375, 370);
	const glm::ivec2 pp261(380, 330);
	const glm::ivec2 pp26(335, 370);
	const glm::ivec2 pp27(330, 340);
	const glm::ivec2 pp28(320, 320);
	const glm::ivec2 pp29(210, 360);
	const glm::ivec2 pp30(200, 350);
	const glm::ivec2 pp31(200, 330);
	const glm::ivec2 pp32(185, 262);
	const glm::ivec2 pp33(145, 350);
	const glm::ivec2 pp34(110, 264);
	const glm::ivec2 pp35(150, 264);
	const glm::ivec2 pp36(150, 208);
	const glm::ivec2 pp37(140, 250);
	const glm::ivec2 pp38(130, 240);
	const glm::ivec2 pp39(140, 230);
	const glm::ivec2 pp40(150, 230);
	const glm::ivec2 pp41(140, 205);
	const glm::ivec2 pp42(170, 215);
	const glm::ivec2 pp43(145, 240);
	const glm::ivec2 pp44(300, 220);
	const glm::ivec2 pp45(165, 300);
	const glm::ivec2 pp46(190, 345);
	const glm::ivec2 pp47(170, 352);
	const glm::ivec2 pp48(150, 355);
	const glm::ivec2 pp49(165, 338);
	const glm::ivec2 pp50(161, 335);
	DrawLine(pp1, pp2, col3);
	DrawLine(pp2, pp3, col3);
	DrawLine(pp3, pp4, col3);
	DrawLine(pp4, pp5, col3);
	DrawLine(pp5, pp6, col3);
	DrawLine(pp6, pp7, col3);
	DrawLine(pp7, pp10, col3);
	DrawLine(pp10, pp12, col3);
	DrawLine(pp12, pp13, col3);
	DrawLine(pp6, pp11, col3);
	DrawLine(pp3, pp8, col3);
	DrawLine(pp4, pp7, col3);
	DrawLine(pp5, pp7, col3);
	DrawLine(pp6, pp7, col3);
	DrawLine(pp2, pp44, col3);
	DrawLine(pp44, pp9, col3);
	DrawLine(pp11, pp261, col3);
	DrawLine(pp261, pp7, col3);
	DrawLine(pp261, pp25, col3);
	DrawLine(pp11, pp14, col3);
	DrawLine(pp11, pp15, col3);
	DrawLine(pp14, pp15, col3);
	DrawLine(pp14, pp16, col3);
	DrawLine(pp15, pp18, col3);
	DrawLine(pp16, pp18, col3);
	DrawLine(pp20, pp18, col3);
	DrawLine(pp20, pp26, col3);
	DrawLine(pp25, pp20, col3);
	DrawLine(pp26, pp27, col3);
	DrawLine(pp27, pp10, col3);
	DrawLine(pp16, pp17, col3);
	DrawLine(pp18, pp19, col3);
	DrawLine(pp17, pp19, col3);
	DrawLine(pp17, pp21, col3);
	DrawLine(pp19, pp22, col3);
	DrawLine(pp21, pp22, col3);
	DrawLine(pp21, pp24, col3);
	DrawLine(pp22, pp23, col3);
	DrawLine(pp24, pp23, col3);
	DrawLine(pp24, pp33, col3);
	DrawLine(pp33, pp34, col3);
	DrawLine(pp23, pp30, col3);
	DrawLine(pp30, pp29, col3);
	DrawLine(pp30, pp31, col3);
	DrawLine(pp29, pp27, col3);
	DrawLine(pp31, pp28, col3);
	DrawLine(pp31, pp32, col3);
	DrawLine(pp32, pp13, col3);
	DrawLine(pp34, pp35, col3);
	DrawLine(pp35, pp45, col3);
	DrawLine(pp34, pp37, col3);
	DrawLine(pp35, pp37, col3);
	DrawLine(pp37, pp38, col3);
	DrawLine(pp38, pp43, col3);
	DrawLine(pp38, pp39, col3);
	DrawLine(pp43, pp42, col3);
	DrawLine(pp42, pp41, col3);
	DrawLine(pp39, pp40, col3);
	DrawLine(pp40, pp41, col3);
	DrawLine(pp36, pp1, col3);
	DrawLine(pp46, pp50, col3);
	DrawLine(pp49, pp47, col3);
	DrawLine(pp46, pp47, col3);
	DrawLine(pp47, pp48, col3);
}

/*OLD RENDERER
void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	// ______________ Drawing the axels ____________________\\ 
	if (scene.GetModelCount() > 0)
	{
		bool axes_flag = scene.GetActiveModel().Get_axes_flag();
		if (false)
			axes_function(half_width, half_height, scene);
	}
	//_____________________________\\ 
	if (scene.GetModelCount() > 0)
	{

		int modles_num = scene.GetModelCount();
		for (int i = 0; i < modles_num; i++)
		{
			MeshModel curr_mesh = scene.GetModel(i);
			DrawMesh(curr_mesh, curr_mesh.Get_Faces(), curr_mesh.Get_vertices(), scene , half_width, half_height,i);

		}

		MeshModel curr_mesh = scene.GetActiveModel();
		DrawMesh(curr_mesh, curr_mesh.Get_Faces(), curr_mesh.Get_vertices(), scene, half_width, half_height,2);

		Scene scene1 = scene;
		if (scene1.getiscolored())
		{
			float camera_depth = scene.GetActiveCamera().Get_Local_transZ();
			if (scene1.get_blur() == true) {
				for (int i = 0; i < viewport_width; i++)
				{
					for (int j = 0; j < viewport_height; j++) {
						if (i > 2 && i < viewport_width - 2 && j>2 && j < viewport_height - 2)
						{
							glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
							for (int ii = i - 2; ii <= i + 2; ii++)
							{
								for (int jj = j - 2; jj <= j + 2; jj++)
								{
									color += temp_color_buffer[Z_INDEX(viewport_width, ii, jj)];
								}
							}
							color = color / glm::vec3(25.0f, 25.0f, 25.0f);
							PutPixel(i, j, color);
						}
					}
				}
			}
			for (int i = 0; i < viewport_width; i++)
			{
				for (int j = 0; j < viewport_height; j++)
				{
					if (z_buffer[Z_INDEX(viewport_width, i, j)] != -INFINITY)
					{
						float alpha = 1 - (10 - z_buffer[Z_INDEX(viewport_width, i, j)]) / (20);
						if (scene1.get_fog() == true)
						{
							float fog = (scene1.get_fog_end() - abs(camera_depth - z_buffer[Z_INDEX(viewport_width, i, j)])) / (scene1.get_fog_end() - scene1.get_fog_start());
							PutPixel(i, j, (1 - fog) * glm::vec3(0.5) + (fog)*temp_color_buffer[Z_INDEX(viewport_width, i, j)]);
						}
						else
						{
							PutPixel(i, j, alpha * temp_color_buffer[Z_INDEX(viewport_width, i, j)]);
						}
						z_buffer[Z_INDEX(viewport_width, i, j)] = -INFINITY;
						temp_color_buffer[Z_INDEX(viewport_width, i, j)] = glm::vec3(0.8, 0.8, 0.8);
					}

				}
			}
			max_z = -1 * INFINITY;
			min_z = INFINITY;
		}
	}
	if (scene.GetDrawCamera() == true)
	{
		Scene scene1 = scene;
		for (int i = 0; i < scene.GetCameraCount(); i++)
		{
			MeshModel& camera_ver = *(scene.camera_model);
			Camera& currCamera = scene1.GetCamera(i);
			Camera& activeCamera = scene1.GetActiveCamera();
			std::vector<glm::vec3> camera_vertices = camera_ver.Get_vertices();
			glm::mat4x4 trans = activeCamera.GetViewTransformation();

			if (i != scene.GetActiveCameraIndex())
			{
				for (int j = 0; j < camera_ver.GetFacesCount(); j++) {

					const Face& model_face = camera_ver.GetFace(j);
					glm::vec3 u0 = camera_vertices[model_face.GetVertexIndex(0) - 1];
					glm::vec3 u1 = camera_vertices[model_face.GetVertexIndex(1) - 1];
					glm::vec3 u2 = camera_vertices[model_face.GetVertexIndex(2) - 1];
					u0 *= 1000;
					u1 *= 1000;
					u2 *= 1000;
					glm::vec4 v0 = trans * glm::vec4(u0, 1);
					glm::vec4 v1 = trans * glm::vec4(u1, 1);
					glm::vec4 v2 = trans * glm::vec4(u2, 1);
					v0 /= v0.w ? v0.w : 1;
					v1 /= v1.w ? v1.w : 1;
					v2 /= v2.w ? v2.w : 1;
					v0.x += viewport_width / 2;
					v1.x += viewport_width / 2;
					v2.x += viewport_width / 2;
					v0.y += viewport_height / 2;
					v1.y += viewport_height / 2;
					v2.y += viewport_height / 2;
					DrawLine(v0, v1, glm::vec3(0, 0, 0));
					DrawLine(v0, v2, glm::vec3(0, 0, 0));
					DrawLine(v2, v1, glm::vec3(0, 0, 0));

				}
			}
		}
	}
	if (scene.GetLightsNumber() > 0)
	{
		for (int i = 0; i < scene.GetLightsNumber(); i++)
		{
			Lights light = scene.GetLight(i);
			DrawLight(light.GetLocationVec().x, light.GetLocationVec().y);
		}
	}
}*/

//New Renderer

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	int camera_count = scene->GetCameraCount();
	if (camera_count > 0)
	{
		const Camera& active_camera = scene->GetActiveCamera();
		int model_count = scene->GetModelCount();
		if (model_count > 0)
		{
			for (int model_index = 0; model_index < model_count; model_index++)
			{
				std::shared_ptr<MeshModel> currentModel = scene->GetModel(model_index);
				// Activate the 'colorShader' program (vertex and fragment shaders)

				shader.use();
				// ... set view and projection matrix
				skyboxShader.setUniform("view", active_camera.GetViewTransformation());
				skyboxShader.setUniform("projection", active_camera.GetProjectionTransformation());
				glBindVertexArray(cubeVAO);
				glBindTexture(GL_TEXTURE_2D, cubeMapTexture);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
				colorShader.use();
				// Set the uniform variables
				colorShader.setUniform("model", currentModel->GetWorldTransformation() * currentModel->GetModelTransformation());
				colorShader.setUniform("view", active_camera.GetViewTransformation());
				colorShader.setUniform("projection", active_camera.GetProjectionTransformation());
				colorShader.setUniform("is_mesh", 1);
				colorShader.setUniform("mapping_type", scene->GetActiveModel()->GetTextureMappingType());
				colorShader.setUniform("center_cor", scene->GetActiveModel()->GetCenter());
				colorShader.setUniform("max_z", scene->GetActiveModel()->max_z);
				colorShader.setUniform("material.textureMap", 0);
				colorShader.setUniform("enviro_map", 1);
				colorShader.setUniform("material.k_i", scene->GetActiveModel()->get_ambient());
				colorShader.setUniform("material.k_d", scene->GetActiveModel()->get_diffuse());
				colorShader.setUniform("material.k_s", scene->GetActiveModel()->get_diffuse());
				colorShader.setUniform("camera_pos", scene->GetActiveCamera().GetEye());
				colorShader.setUniform("type", scene->GetMappingType());
				colorShader.setUniform("enviro_map", 2);
				//----Texture----
				texture1.bind(0);

				//drawing the model
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
				//------------------
				texture1.unbind(0);
				////------------------
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//glBindVertexArray(currentModel->GetVAO());
				//glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				//glBindVertexArray(0);
			}
		}
		lightcount = scene->GetLightsNumber();
		if (lightcount)
		{
			const Camera& active_camera = scene->GetActiveCamera();
			for (int i = 0; i < scene->GetLightsNumber(); i++)
			{
				lightShader.use();
				Lights activelight = scene->GetLight(i);
				// Activate the 'colorShader' program (vertex and fragment shaders)
				// Set the uniform variables
				//colorShader.setUniform("lightposition", activelight.getlightvert());
				colorShader.setUniform("view", active_camera.GetViewTransformation());
				colorShader.setUniform("projection", active_camera.GetProjectionTransformation());
				colorShader.setUniform("is_mesh", 0);
				if (scene->GetLightsNumber())
				{
					colorShader.setUniform("light.alpha", activelight.alpha);
					colorShader.setUniform("light.l_i", activelight.ambient_color);
					colorShader.setUniform("light.l_d", activelight.diffuse_color);
					colorShader.setUniform("light.l_s", activelight.specular_color);
					colorShader.setUniform("light.pos", activelight.location);
				}

			}
		}
		//Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		glm::mat4 view = glm::mat4(glm::mat3(active_camera.GetViewTransformation()));	// Remove any translation component of the view matrix

		skyboxShader.setUniform("view", view);
		skyboxShader.setUniform("projection", active_camera.GetProjectionTransformation());

		// skybox cube
		texture3.bind(cubeMapTexture);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
		texture3.unbind(cubeMapTexture);
	}

}

/*
void Renderer::DrawMesh(MeshModel mesh, std::vector<Face> faces, std::vector<glm::vec3> vertices, Scene scene, int width, int height, int index)
{
	// returning the values to  0 to remove the centering trick
	const int HALF_WIDTH = 0;
	const int HALF_HEIGHT = 0;
	if (true)//world 
	{
		int face_count = faces.size();
		glm::vec3 p1, p2, p3, facecolor;
		glm::mat4 T_mat = mesh.Get_trans_mat();
		glm::mat4x4 camera_transform = scene.GetActiveCamera().GetViewTransformation();
		glm::mat4x4 worldaxesmodel_transform = camera_transform*mesh.Get_world_translate_mat() * mesh.Get_worldX_rotation() * mesh.Get_worldY_rotation() * mesh.Get_worldZ_rotation() * mesh.Get_world_scale_mat() * mesh.Get_local_translate_mat() * mesh.Get_local_scale_mat();
		T_mat = camera_transform * T_mat;

		for (int i = 0; i < face_count; i++)
		{
			Face face = faces[i];
			p1 = vertices[face.GetVertexIndex(0) - 1];
			p2 = vertices[face.GetVertexIndex(1) - 1];
			p3 = vertices[face.GetVertexIndex(2) - 1];
			//calculating temp vectors
			glm::vec4 temp_vec1 = glm::vec4(p1, 1);
			temp_vec1 = T_mat * temp_vec1;
			glm::vec4 temp_vec2 = glm::vec4(p2, 1);
			temp_vec2 = T_mat * temp_vec2;
			glm::vec4 temp_vec3 = glm::vec4(p3, 1);
			temp_vec3 = T_mat * temp_vec3;
			//calculating points cordinations of X-axis
			p1.x = temp_vec1.x / temp_vec1.w;
			p2.x = temp_vec2.x / temp_vec2.w;
			p3.x = temp_vec3.x / temp_vec3.w;
			//moving the center point to the center of the window
			//p1.x += HALF_WIDTH;
			//p2.x += HALF_WIDTH;
			//p3.x += HALF_WIDTH;
			//calculating points cordinations of Y-axis
			p1.y = temp_vec1.y / temp_vec1.w;
			p2.y = temp_vec2.y / temp_vec2.w;
			p3.y = temp_vec3.y / temp_vec3.w;
			//moving the center point to the center of the window
			//p1.y += HALF_HEIGHT;
			//p2.y += HALF_HEIGHT;
			//p3.y += HALF_HEIGHT;
			//calculating points cordinations of Z-axis
			p1.z = temp_vec1.z / temp_vec1.w;
			p2.z = temp_vec2.z / temp_vec2.w;
			p3.z = temp_vec3.z / 1;
			//Drawing the lines between the points with black color
			p1.x = (p1.x + 1) * width;
			p1.y = (p1.y + 1) * height;
			p2.x = (p2.x + 1) * width;
			p2.y = (p2.y + 1) * height;
			p3.x = (p3.x + 1) * width;
			p3.y = (p3.y + 1) * height;
			glm::vec3 color = glm::vec3(0, 0, 0);
			DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y), color);
			DrawLine(glm::vec2(p2.x, p2.y), glm::vec2(p3.x, p3.y), color);
			DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p3.x, p3.y), color);
			int max_x = std::max(p1.x, std::max(p2.x, p3.x));
			int max_y = std::max(p1.y, std::max(p2.y, p3.y));
			int min_x = std::min(p1.x, std::min(p2.x, p3.x));
			int min_y = std::min(p1.y, std::min(p2.y, p3.y));
			if (scene.getiscolored())
			{
				if (scene.GetLightsNumber() > 0)
				{

					glm::vec3 face_normal = Utils::CalcNormal(p1, p2, p3);
					if (scene.get_shading_type() == 0)
					{
						glm::vec3 cenetr = p1 + p2 + p3;
						cenetr /= 3;
						sc_func(p1, p2, p3, min_x, max_x, min_y, max_y, getColor(cenetr, face_normal, scene, mesh));
					}
					else
					{
						glm::vec3 p1_normal = mesh.Get_normals(face.GetNormalIndex(0) - 1);
						glm::vec3 p2_normal = mesh.Get_normals(face.GetNormalIndex(1) - 1);
						glm::vec3 p3_normal = mesh.Get_normals(face.GetNormalIndex(2) - 1);
						glm::vec4 temp_vec = glm::vec4(p1_normal, 1);
						temp_vec = T_mat * temp_vec;
						p1_normal.x = temp_vec.x / temp_vec.w;
						p1_normal.y = temp_vec.y / temp_vec.w;
						p1_normal.z = temp_vec.z / temp_vec.w;
						temp_vec = glm::vec4(p2_normal, 1);
						temp_vec = T_mat * temp_vec;
						p2_normal.x = temp_vec.x / temp_vec.w;
						p2_normal.y = temp_vec.y / temp_vec.w;
						p2_normal.z = temp_vec.z / temp_vec.w;
						temp_vec = glm::vec4(p3_normal, 1);
						temp_vec = T_mat * temp_vec;
						p3_normal.x = temp_vec.x / temp_vec.w;
						p3_normal.y = temp_vec.y / temp_vec.w;
						p3_normal.z = temp_vec.z / temp_vec.w;
						if (scene.get_shading_type() == 1)
						{
							Phong(p1, p2, p3, min_x, max_x, min_y, max_y, p1_normal, p2_normal, p3_normal, scene, mesh);
						}
					}
				}
				else
				{
					sc_func(p1, p2, p3, min_x, max_x, min_y, max_y, mesh.GetColorVec(0));
				}

			}
			// drawing the axes
			if (mesh.Get_axes_flag() == true)
			{
				float max_x = mesh.Get_max_x();
				float max_y = mesh.Get_max_y();
				float max_z = mesh.Get_max_z();
				float min_x = mesh.Get_min_x();
				float min_y = mesh.Get_min_y();
				float min_z = mesh.Get_min_z();
				glm::vec3 px_max = glm::vec3(max_x, (max_y + min_y) / 2, (max_z + min_z) / 2);
				glm::vec3 py_max = glm::vec3((max_x + min_x) / 2, max_y, (max_z + min_z) / 2);
				glm::vec3 pz_max = glm::vec3((max_x + min_x) / 2, (max_y + min_y) / 2, max_z);
				glm::vec3 center = glm::vec3((max_x + min_x) / 2, (max_y + min_y) / 2, (max_z + min_z) / 2);
				if (mesh.Get_LorW() == false)
				{
					glm::vec4 temp_vec = glm::vec4(px_max, 1);
					temp_vec = T_mat * temp_vec;
					px_max = temp_vec;
					temp_vec = glm::vec4(py_max, 1);
					temp_vec = T_mat * temp_vec;
					py_max = temp_vec;
					temp_vec = glm::vec4(pz_max, 1);
					temp_vec = T_mat * temp_vec;
					pz_max = temp_vec;
					temp_vec = glm::vec4(center, 1);
					temp_vec = T_mat * temp_vec;
					center = temp_vec;
				}
				else
				{
					glm::vec4 temp_vec1 = glm::vec4(px_max, 1);
					temp_vec1 = worldaxesmodel_transform * temp_vec1;
					px_max = temp_vec1;
					glm::vec4 temp_vec2 = glm::vec4(py_max, 1);
					temp_vec2 = worldaxesmodel_transform * temp_vec2;
					py_max = temp_vec2;
					glm::vec4 temp_vec3 = glm::vec4(pz_max, 1);
					temp_vec3 = worldaxesmodel_transform * temp_vec3;
					pz_max = temp_vec3;
					glm::vec4 temp_vec4 = glm::vec4(center, 1);
					temp_vec4 = worldaxesmodel_transform * temp_vec4;
					center = temp_vec4;
				}
				px_max.x = (px_max.x + 1) * width;
				px_max.y = (px_max.y + 1) * height;
				py_max.x = (py_max.x + 1) * width;
				py_max.y = (py_max.y + 1) * height;
				pz_max.x = (pz_max.x + 1) * width;
				pz_max.y = (pz_max.y + 1) * height;
				center.x = (center.x + 1) * width;
				center.y = (center.y + 1) * height;

				DrawLine(glm::vec2(px_max.x, px_max.y), center, glm::vec3(0, 1, 0));
				DrawLine(glm::vec2(py_max.x, py_max.y), center, glm::vec3(0, 1, 0));
				DrawLine(glm::vec2(pz_max.x, pz_max.y), center, glm::vec3(0, 1, 0));

			}
			if (mesh.GetTrianglebox())
			{
				facecolor = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
				glm::vec2 left_top = glm::vec2(min_x, max_y);
				glm::vec2 left_bot = glm::vec2(min_x, min_y);
				glm::vec2 right_top = glm::vec2(max_x, max_y);
				glm::vec2 right_bot = glm::vec2(max_x, min_y);
				DrawLine(right_top, right_bot, facecolor);
				DrawLine(right_top, left_top, facecolor);
				DrawLine(left_bot, right_bot, facecolor);
				DrawLine(left_top, left_bot, facecolor);
			}
			if (mesh.Get_box_flag() == true)
			{
				float max_x = mesh.Get_max_x();
				float max_y = mesh.Get_max_y();
				float max_z = mesh.Get_max_z();
				float min_x = mesh.Get_min_x();
				float min_y = mesh.Get_min_y();
				float min_z = mesh.Get_min_z();
				//---------down face------------
				glm::vec3 p1 = glm::vec3(max_x, min_y, min_z);
				glm::vec3 p2 = glm::vec3(max_x, min_y, max_z);
				glm::vec3 p3 = glm::vec3(min_x, min_y, min_z);
				glm::vec3 p4 = glm::vec3(min_x, min_y, max_z);
				//---------upper face----------
				glm::vec3 p5 = glm::vec3(max_x, max_y, min_z);
				glm::vec3 p6 = glm::vec3(max_x, max_y, max_z);
				glm::vec3 p7 = glm::vec3(min_x, max_y, min_z);
				glm::vec3 p8 = glm::vec3(min_x, max_y, max_z);
				//----------------------------------------
				glm::vec4 temp_vec = glm::vec4(p1, 1);
				temp_vec = T_mat * temp_vec;
				p1.x = temp_vec.x / temp_vec.w;
				p1.y = temp_vec.y / temp_vec.w;
				p1.z = temp_vec.z / temp_vec.w;
				//----------------------------------------
				temp_vec = glm::vec4(p2, 1);
				temp_vec = T_mat * temp_vec;
				p2.x = temp_vec.x / temp_vec.w;
				p2.y = temp_vec.y / temp_vec.w;
				p2.z = temp_vec.x / temp_vec.w;
				//----------------------------------------
				temp_vec = glm::vec4(p3, 1);
				temp_vec = T_mat * temp_vec;
				p3.x = temp_vec.x / temp_vec.w;
				p3.y = temp_vec.y / temp_vec.w;
				p3.z = temp_vec.z / temp_vec.w;
				//----------------------------------------
				temp_vec = glm::vec4(p4, 1);
				temp_vec = T_mat * temp_vec;
				p4.x = temp_vec.x / temp_vec.w;
				p4.y = temp_vec.y / temp_vec.w;
				p4.z = temp_vec.z / temp_vec.w;
				//----------------------------------------
				temp_vec = glm::vec4(p5, 1);
				temp_vec = T_mat * temp_vec;
				p5.x = temp_vec.x / temp_vec.w;
				p5.y = temp_vec.y / temp_vec.w;
				p5.z = temp_vec.z / temp_vec.w;
				//----------------------------------------
				temp_vec = glm::vec4(p6, 1);
				temp_vec = T_mat * temp_vec;
				p6.x = temp_vec.x / temp_vec.w;
				p6.y = temp_vec.y / temp_vec.w;
				p6.z = temp_vec.z / temp_vec.w;
				//----------------------------------------
				temp_vec = glm::vec4(p7, 1);
				temp_vec = T_mat * temp_vec;
				p7.x = temp_vec.x / temp_vec.w;
				p7.y = temp_vec.y / temp_vec.w;
				p7.z = temp_vec.z / temp_vec.w;
				//----------------------------------------
				temp_vec = glm::vec4(p8, 1);
				temp_vec = T_mat * temp_vec;
				p8.x = temp_vec.x / temp_vec.w;
				p8.y = temp_vec.y / temp_vec.w;
				p8.z = temp_vec.z / temp_vec.w;
				//---------------------------------------
				p1.x = (p1.x + 1) * width;
				p1.y = (p1.y + 1) * height;
				p2.x = (p2.x + 1) * width;;
				p2.y = (p2.y + 1) * height;
				p3.x = (p3.x + 1) * width;;
				p3.y = (p3.y + 1) * height;
				p4.x = (p4.x + 1) * width;
				p4.y = (p4.y + 1) * height;
				p5.x = (p5.x + 1) * width;;
				p5.y = (p5.y + 1) * height;
				p6.x = (p6.x + 1) * width;;
				p6.y = (p6.y + 1) * height;
				p7.x = (p7.x + 1) * width;;
				p7.y = (p7.y + 1) * height;
				p8.x = (p8.x + 1) * width;;
				p8.y = (p8.y + 1) * height;
				//---------drawing the bottom face----------
				DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p3.x, p3.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p2.x, p2.y), glm::vec2(p4.x, p4.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p3.x, p3.y), glm::vec2(p4.x, p4.y), glm::vec3(1, 0, 1));
				//-------drawing the upper face-------------
				DrawLine(glm::vec2(p5.x, p5.y), glm::vec2(p6.x, p6.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p5.x, p5.y), glm::vec2(p7.x, p7.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p6.x, p6.y), glm::vec2(p8.x, p8.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p7.x, p7.y), glm::vec2(p8.x, p8.y), glm::vec3(1, 0, 1));
				//------drawing right side face----------------
				DrawLine(glm::vec2(p6.x, p6.y), glm::vec2(p2.x, p2.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p5.x, p5.y), glm::vec3(1, 0, 1));
				//------drawing left side face-----------------
				DrawLine(glm::vec2(p7.x, p7.y), glm::vec2(p3.x, p3.y), glm::vec3(1, 0, 1));
				DrawLine(glm::vec2(p8.x, p8.y), glm::vec2(p4.x, p4.y), glm::vec3(1, 0, 1));
			}
			if (mesh.Get_faces_flag() == true)
			{
				glm::vec3 edge1 = p2 - p1;
				glm::vec3 edge2 = p3 - p1;
				glm::vec3 face_normal = glm::cross(edge1, edge2);
				face_normal = glm::normalize(face_normal);
				face_normal *= 69;
				glm::vec3 center = p1 + p2 + p3;
				center /= 3;
				DrawLine(glm::vec2(center), glm::vec2(face_normal.x + p1.x, face_normal.y + p1.y), glm::vec3(0.3, 0.8, 0.3));
			}
			if (mesh.Get_vertices_flag() == true)
			{
				glm::vec3 normal = mesh.Get_normals(face.GetNormalIndex(0) - 1);
				normal *= 69;
				DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p1.x + normal.x, p1.y + normal.y), glm::vec3(0.9, 0.5, 0.9));
				normal = mesh.Get_normals(face.GetNormalIndex(1) - 1);
				normal *= 69;
				DrawLine(glm::vec2(p2.x, p2.y), glm::vec2(p2.x + normal.x, p2.y + normal.y), glm::vec3(0.9, 0.5, 0.9));
				normal = mesh.Get_normals(face.GetNormalIndex(2) - 1);
				normal *= 69;
				DrawLine(glm::vec2(p3.x, p3.y), glm::vec2(p3.x + normal.x, p3.y + normal.y), glm::vec3(0.9, 0.5, 0.9));
			}
		}
	}
	else//local
	{
		int face_count = faces.size();
		glm::vec3 p1, p2, p3;
		glm::mat4 T_mat = mesh.Get_trans_mat();
		for (int i = 0; i < face_count; i++)
		{
			Face face = faces[i];
			p1 = vertices[face.GetVertexIndex(0) - 1];
			p2 = vertices[face.GetVertexIndex(1) - 1];
			p3 = vertices[face.GetVertexIndex(2) - 1];			//calculating temp vectors
			glm::vec4 temp_vec1 = glm::vec4(p1, 1);
			temp_vec1 = T_mat * temp_vec1;
			glm::vec4 temp_vec2 = glm::vec4(p2, 1);
			temp_vec2 = T_mat * temp_vec2;
			glm::vec4 temp_vec3 = glm::vec4(p3, 1);
			temp_vec3 = T_mat * temp_vec3;
			//calculating points cordinations of X-axis
			p1.x = temp_vec1.x / temp_vec1.w;
			p2.x = temp_vec2.x / temp_vec2.w;
			p3.x = temp_vec3.x / temp_vec3.w;
			//moving the center point to the center of the window
			p1.x += HALF_WIDTH;
			p2.x += HALF_WIDTH;
			p3.x += HALF_WIDTH;
			//calculating points cordinations of Y-axis
			p1.y = temp_vec1.y / temp_vec1.w;
			p2.y = temp_vec2.y / temp_vec2.w;
			p3.y = temp_vec3.y / temp_vec3.w;
			//moving the center point to the center of the windo
			p1.y += HALF_HEIGHT;
			p2.y += HALF_HEIGHT;
			p3.y += HALF_HEIGHT;
			//calculating points cordinations of Z-axis
			p1.z = temp_vec1.z / temp_vec1.w;
			p2.z = temp_vec2.z / temp_vec2.w;
			p3.z = temp_vec3.z / 1;
			//Drawing the lines between the points with black color
			DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y), glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(p2.x, p2.y), glm::vec2(p3.x, p3.y), glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p3.x, p3.y), glm::vec3(0, 0, 0));
		}
	}
}*/

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}

/*
void Renderer::axes_function(int half_width, int half_height, Scene scene) {
	int l_w = scene.GetActiveCamera().Get_LorW();
	if (true) {
		glm::vec3 zero_point = glm::vec3(0, 0, 0);
		glm::vec3 x_axis = glm::vec3(half_width, 0, 0);
		glm::vec3 y_axis = glm::vec3(0, half_height, 0);
		glm::vec3 z_axis = glm::vec3(0, 0, 300);
		glm::mat4x4 camera_transform = scene.GetActiveCamera().GetViewTransformation();
		glm::vec4 temp_vec1 = glm::vec4(zero_point, 1);
		temp_vec1 = camera_transform * temp_vec1;
		zero_point = temp_vec1;
		glm::vec4 temp_vec2 = glm::vec4(x_axis, 1);
		temp_vec2 = camera_transform * temp_vec2;
		x_axis = temp_vec2;
		glm::vec4 temp_vec3 = glm::vec4(y_axis, 1);
		temp_vec3 = camera_transform * temp_vec3;
		y_axis = temp_vec3;
		glm::vec4 temp_vec4 = glm::vec4(z_axis, 1);
		temp_vec4 = camera_transform * temp_vec4;
		z_axis = temp_vec4;
		zero_point.x = (zero_point.x + 1) * half_width;
		zero_point.y = (zero_point.y + 1) * half_height;
		x_axis.x = half_width * (x_axis.x + 1);
		y_axis.x = half_width * (y_axis.x + 1);
		z_axis.x = half_width * (z_axis.x + 1);
		x_axis.y = half_height * (x_axis.y + 1);
		y_axis.y = half_height * (y_axis.y + 1);
		z_axis.y = half_height * (z_axis.y + 1);
		DrawLine(zero_point, x_axis, glm::vec3(1, 0, 0));
		DrawLine(zero_point, y_axis, glm::vec3(1, 0, 0));
		DrawLine(zero_point, z_axis, glm::vec3(1, 0, 0));
	}
	else {

	}
}*/
/*
void Renderer::sc_func(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int min_x, int max_x, int min_y, int max_y, glm::vec3 color)
{
	for (int i = min_x; i < max_x; i++)
	{
		for (int j = min_y; j < max_y; j++)
		{
			if (Utils::triangle_flag(glm::vec2(i, j), p1, p2, p3))
			{

				float z1 = p1.z, z2 = p2.z, z3 = p3.z, A, A1, A2, A3, z;

				A1 = 0.5 * abs(i * p2.y + p2.x * p3.y + p3.x * j - j * p2.x - p2.y * p3.x - p3.y * i);
				A3 = 0.5 * abs(p1.x * p2.y + p2.x * j + p3.x * p1.y - p1.y * p2.x - p2.y * i - j * p1.x);
				A2 = 0.5 * abs(p1.x * j + i * p3.y + p3.x * p1.y - p1.y * i - j * p3.x - p3.y * p1.x);
				A = A1 + A2 + A3;
				z = A1 / A * z1 + A2 / A * z2 + A3 / A * z3;
				if (z > max_z) {
					max_z = z;
				}
				if (z < min_z)
				{
					min_z = z;
				}
				if ((i < viewport_width && j < viewport_height && i>0 && j>0) && z >= z_buffer[Z_INDEX(viewport_width, i, j)])
				{
					z_buffer[Z_INDEX(viewport_width, i, j)] = z;
					temp_color_buffer[Z_INDEX(viewport_width, i, j)] = color;
				}
			}
		}
	}
}*/
/*
glm::vec3 Renderer::CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 edge1 = p2 - p1;
	glm::vec3 edge2 = p3 - p1;
	glm::vec3 face_normal = glm::cross(edge1, edge2);
	face_normal = glm::normalize(face_normal);
	return face_normal;
}*/
/*
void Renderer::Phong(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int minx, int maxx, int miny, int maxy, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3, Scene scene, MeshModel mesh)
{
	glm::vec3 color = glm::vec3(0.0f);
	float T1, T2, T3, T;
	for (int i = minx; i < maxx; i++)
	{
		for (int j = miny; j < maxy; j++)
		{
			if (Utils::triangle_flag(glm::vec2(i, j), p1, p2, p3))
			{

				float z1 = p1.z, z2 = p2.z, z3 = p3.z, A, A1, A2, A3, z;

				T1 = Utils::TriangleCalc(i, j, p2.x, p2.y, p3.x, p3.y);
				T2 = Utils::TriangleCalc(i, j, p1.x, p1.y, p3.x, p3.y);
				T3 = Utils::TriangleCalc(i, j, p1.x, p1.y, p2.x, p2.y);
				T = T1 + T2 + T3;
				z = T1 / T * z1 + T2 / T * z2 + T3 / T * z3;
				if (z > max_z) {
					max_z = z;
				}
				if (z < min_z)
				{
					min_z = z;
				}
				glm::vec3 interpolated_normal = Utils::InterpolateNormal(p1, p2, p3, glm::vec3(i, j, z), n1, n2, n3, T1, T2, T3);
				color = getColor(glm::vec3(i, j, z), interpolated_normal, scene, mesh);
				if ((i < viewport_width && j < viewport_height && i>0 && j>0) && z > z_buffer[Z_INDEX(viewport_width, i, j)])
				{
					z_buffer[Z_INDEX(viewport_width, i, j)] = z;
					temp_color_buffer[Z_INDEX(viewport_width, i, j)] = color;
				}
			}
		}
	}
}*/
/*
void Renderer::DrawLight(int centerx, int centery)
{
	for (int i = centerx; i < centerx + 50; i++)
	{
		for (int j = centery; j < centery + 50; j++)
		{
			PutPixel(i, j, glm::vec3(1, 1, 1));
		}
	}
}*/
/*
glm::vec3 Renderer::getColor(glm::vec3 point, glm::vec3 normal, Scene& scene, MeshModel& mesh)
{
	glm::vec3 color = glm::vec3(0.0f);
	for (int i = 0; i < scene.GetLightsNumber(); i++)
	{
		Lights light = scene.GetLight(i);
		//--------ambient---------------------------
		glm::vec3 ia = mesh.ambient_color * light.ambient_color;
		//------------diffuse-------------
		glm::vec3 l;
		glm::vec3 temp = light.GetLocationVec();

		//l = light.GetLocationVec() - point;
		l = point - light.GetLocationVec();
		if (scene.get_shading_type() != 0)
			l = light.GetLocationVec() - point;

		l = glm::normalize(l);
		glm::vec3 n = normal;
		float degree = fmax(glm::dot(l, n), 0.0);
		glm::vec3 id = mesh.diffuse_color * degree * light.diffuse_color;
		id *= 10;
		//-------------specualr------------
		glm::vec3 temp_n = n;
		temp_n *= 2;
		glm::vec3 r = temp_n * glm::dot(n, l) - l;
		glm::vec3 v = normal - scene.GetActiveCamera().GetCameraLocation();
		v = glm::normalize(v);
		float rv = fmax(pow(glm::dot(r, v), 1), 0.0);
		glm::vec3 is = mesh.specular_color * rv * light.specular_color;
		is *= 2;
		//-------- reflections ----------------
		if (scene.get_reflect_ray() == true)
		{
			glm::vec3 temp_r = r;
			temp_r = glm::normalize(temp_r);
			temp_r *= 50;
			glm::vec3 temp_point = point + temp_r;
			DrawLine(point, temp_point, glm::vec3(1, 1, 0));
			glm::vec3 temp_l = light.GetLocationVec() - point;
			temp_l = glm::normalize(temp_l);
			temp_l *= 50;
			DrawLine(point, point + temp_l, glm::vec3(0, 1, 1));
		}
		color += (ia + id + is);
	}
	return color;
}*/


void Renderer::LoadShaders()//loaded at the main
{
	colorShader.loadShaders("vshader.glsl", "fshader.glsl");
	shader.loadShaders("vcubemap.glsl", "fcubemap.glsl");
	skyboxShader.loadShaders("vskybox.glsl", "fskybox.glsl");
}

void Renderer::LoadTextures()//loaded at the main
{
	if (!texture1.loadTexture("C:\\Users\\omari\\Documents\\GitHub\\computer-graphics-2023-faisal_omari\\Data\\crate.jpg", true))
	{
		texture1.loadTexture("C:\\Users\\omari\\Documents\\GitHub\\computer-graphics-2023-faisal_omari\\Data\\crate.jpg", true);
	}
	GLfloat cubeVertices[] =
	{
		// Positions          // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	//shader.use();
	// Setup cube VAO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//skyboxShader.use();
	// Setup skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	std::vector<std::string> faces
	{
		"posx.PNG",
		"negx.PNG",
		"posy.PNG",
		"negy.PNG",
		"posz.PNG",
		"negz.PNG"
	};
	cubeMapTexture = texture3.loadCubemap(faces);

}