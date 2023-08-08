<h1>
By: Faisal Omari - 325616894
(not in pairs)
</h1>
<h2>1)The first branch</h2>
<p>
in the initopenglrendering function we do the following steps:
The steps are as following:
<h4>1)</h4>Using the GLuint pointer, we create the texture in the gl Gen Textures function and return the pointer to the open gl screen tex.
<h4>2)</h4>After that we generate VAO element and pass it's pointer to the gl screen vtc.
<h4>3)</h4>After that we create a new buffer and bind the buffer with it.
<h4>4)</h4>After that we do format to the two vectors as following:
for tex: we set the coordinatews according to the texture map, and bind the GL_array-buffeer to it by creating the memory needed fort it to store our data.
for the vtc: we set the coordinates of the vector to be equal to the vertices.
<h4>5)</h4>After that we intiate the fragment and the vshader by making them ready to work and pass it to the OpenGL so it can begin using it and by passing the place of the vertecies coordinates accordingly with the texture's one and the shader will draw them according to the algorithm on it and then it will be displayeed in the screen.
</p>
<h2>2)Update the mesh model class to load the mesh on the gpu.</h2>
<p>
By using the OpenGL project as a refference, and after we've inserted the VAO adn VBO to the meshmodel that we created in the first of the main before the code, and that we've intialized, so accordingly the VAO will point twoard the VBO which in it's rule gonna be binded and intialized according to the OpenGL and after that the buffer is gonna be created and passes as a reference to the VBO and binded with it and store the vertices values to it as we explained earlier, and we can see this happening in the following image where the gl Draw Arrays is gonna be used to draw the mesh. 
</p>

<h2>3)V_Shader (vertex shader) writing:</h2>
after we copied the reference code, there were a lot of changes to be done to the original code so it can work and be compiatable with the new code, and that were done, after that we got the following code as the implementation of the Vshader:
<code>
    #version 330 core
    layout(location = 0) in vec3 pos;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 texCoords;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main()
    {
        gl_Position = projection * view *  model * vec4(pos, 1.0f);
    }
</code>
note* that the image includes a light window from the previous assignment.
<img src="https://user-images.githubusercontent.com/75030682/221659821-63b32b4a-5131-4732-8b8e-0a3f30fc5a8b.png">

<h2>4)The fregmant shader (Fshader) implementation:</h2>
<code>
    #version 330 core
    out vec4 frag_color;
    void main()
    {
        frag_color = vec4(0.5,0.5,0.5,1);
    }
</code>
<img src="https://user-images.githubusercontent.com/75030682/221659829-9f9d4c01-02b6-4796-a69b-b03ff89aed44.png">

<h2>5)After updating all the relevant vertex attributes and uniforms, we got the following code implementation:</h2>
<code>
    void Renderer::Render(const std::shared_ptr<Scene>& scene)
    {
        int half_width = viewport_width/2;
        int half_height = viewport_height/2;
        int counter_camera = scene->GetCameraCount();
        if (counter_camera > 0)
        {
            int counter_models = scene->GetModelCount();
            if (counter_models > 0)
            {
                const Camera& active_camera = scene->GetActiveCamera();
                for (int model_index = 0; model_index < counter_models; model_index++)
                {
                    std::shared_ptr<MeshModel> curr = scene->GetModel(model_index);
                    colorShader.use();
                    colorShader.setUniform("model", curr->GetWorldTransformation()*curr->GetModelTransformation());
                    colorShader.setUniform("view", active_camera.GetViewTransformation());
                    colorShader.setUniform("projection", active_camera.GetProjectionTransformation());
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glBindVertexArray(curr->GetVAO());
                    glDrawArrays(GL_TRIANGLES, 0, curr->GetModelVertices().size());
                    glBindVertexArray(0);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glBindVertexArray(curr->GetVAO());
                    glDrawArrays(GL_TRIANGLES, 0, curr->GetModelVertices().size());
                    glBindVertexArray(0);
                }
            }
        }
    }
</code>

<h2>6)Phong shading implementation:</h2>
<p>after implementing the phong shader according to the shaders we got, now we remove the previous phong shading implementation from the previous assignment, and make it work according to the new code and shaders implementation, we got the following images as an example after loading one of the meshes from the link(Armaledo):</p>
<img src="https://user-images.githubusercontent.com/75030682/221659831-2676d6ee-69f7-4349-b034-85c76bab7af3.png">
<img src="https://user-images.githubusercontent.com/75030682/221659833-c3628594-01fd-49fd-ae9b-4f358b753296.png">


<h2>7)After implementing the texture mapping, I enabled the canonical project of type Plane, we got the following results:</h2>
Before Planer: (Guko)
<img src="https://user-images.githubusercontent.com/75030682/221659836-fc13c02c-abba-4f6c-9151-3b265bfb485b.png">
After Planer: (of the given crate texture)
<img src="https://user-images.githubusercontent.com/75030682/221659844-8700d3e9-c4ef-4df5-a9a6-b2ff625b48d3.png">


<h2>8)I implemented Nomral mapping: </h2>
<p>I have used material swapping method to implement the normal mapping and by using phong shading we implemented previously and by using different normals of the verticees and faces in the calculations, I got the following results:</p>
Before:
<img src="https://user-images.githubusercontent.com/75030682/221659910-fa5b0082-6c7e-4ecd-b9e1-fbab4e3122cf.png">
After:
<img src="https://user-images.githubusercontent.com/75030682/221659915-69d559e9-e595-49ab-b0b6-53977e86a320.png">

