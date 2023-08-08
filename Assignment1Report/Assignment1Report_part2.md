<h1 style="background-color:DodgerBlue">Assignment 1  -- part2 Final report</h1>
<h2>1) Loading the mesh and printing the faces and vertices </h2>
<p> The image of loading the demo.obj mesh</p>
<img width="579" alt="1" src="https://user-images.githubusercontent.com/75030682/204105397-0af42dc3-82fa-4ab5-8e54-30519c449faa.png">
<h2>2) Scaling and translating the vertices to fit into the window, Done </h2>
<img width="959" alt="2 1" src="https://user-images.githubusercontent.com/75030682/204105471-c5e2d8bc-7433-4d57-a484-ce5dba38e046.png">
The scale is done by the sliders, the determined scale for x and y and z which is now 50, is done by multyplying the verices coordinats by the scale.
the translate is done by adding the HALF_WIDTH and HALF_HIEGHT to every point's x and y coordinats accordingly as shown in the follwing code:
<pre><code>
            // for x coordinats
			p1.x += HALF_WIDTH;
			p2.x += HALF_WIDTH;
			p3.x += HALF_WIDTH;
            // for y coordinats
                        p1.y += HALF_HEIGHT;
			p2.y += HALF_HEIGHT;
			p3.y += HALF_HEIGHT;
</code></pre>
<h2>3) as shown in the previous branch, </h2>
<h3>A screenshoot of another object loaded into the window:</h3>
<img width="959" alt="3 1" src="https://user-images.githubusercontent.com/75030682/204105762-1c2a21f2-8147-467c-a10d-5f00e3616b1c.png">
The drawline code:
<pre><code>
			DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y), glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(p2.x, p2.y), glm::vec2(p3.x, p3.y), glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(p1.x, p1.y), glm::vec2(p3.x, p3.y), glm::vec3(0, 0, 0));
</code></pre>
<h2>4)The GUI items to specify the local and world transformation: </h2>
<p>The following two picture is showing the two windows created to control the scale, translation, and rotation of the object in the Loacl and in the World model:</p>
<h4>The local controls sliders:</h4>
<img width="162" alt="4-1 local controls" src="https://user-images.githubusercontent.com/75030682/204106524-dde784ce-abd3-468b-9d9d-6c75eee6b922.png">
<h4>The world controls sliders:</h4>
<img width="162" alt="4-2 world controls" src="https://user-images.githubusercontent.com/75030682/204106528-f268f8bd-54f9-482b-a135-3a549121ea4a.png">
and accordingly this is the final design of the window with an object loaded for example:
<img width="644" alt="4-3 window picture" src="https://user-images.githubusercontent.com/75030682/204106534-c6097032-62ea-4c4c-abda-c73f5ef2f7f7.png">
the transformation is calculated using the MESHModel.h and cpp files, right there, added all the function to set and get the scale, rotation and translation of x-axis, y-axis, and z-axis, so when the sliders in the GUI is moved, the matrices and vectors is updated accordingly to the function, and setted to the new value, in addition, to Draw the final view of the object with the whole scale, rotation and translation applied to the points, there is a transformation matrix that created, it has the final results of merging the values of the x-axis, y-axis and z-axis of the rotation, and translation matrix with the scaling matrix of the world and local, and it is the result of multiplying them in the following order:
<pre><code>
return world_mat_trans * world_mat_scale * world_matX_rotation * world_matY_rotation * world_matZ_rotation * local_mat_trans * local_matX_rotation * local_matY_rotation * local_matZ_rotation * local_mat_scale;
</code></pre>
and according to this transformation matrix what is done next is as following:
1) Multiplying every vertex of every face by the transformation function (after converting the vertex to be a vector of size 4, the forth value is 1 before multiplying with the matrix)
<pre><code>
			glm::vec4 temp_vec1 = glm::vec4(p1, 1);
			temp_vec1 = T_mat * temp_vec1;
			glm::vec4 temp_vec2 = glm::vec4(p2, 1);
			temp_vec2 = T_mat * temp_vec2;
			glm::vec4 temp_vec3 = glm::vec4(p3, 1);
			temp_vec3 = T_mat * temp_vec3;
</code></pre>
2) Normalizing the vector by dividing it with the forth value of the vector
<pre><code>
			p1.x = temp_vec1.x / temp_vec1.w;
			p2.x = temp_vec2.x / temp_vec2.w;
			p3.x = temp_vec3.x / temp_vec3.w;
            p1.y = temp_vec1.y / temp_vec1.w;
			p2.y = temp_vec2.y / temp_vec2.w;
			p3.y = temp_vec3.y / temp_vec3.w;
			p1.z = temp_vec1.z / temp_vec1.w;
			p2.z = temp_vec2.z / temp_vec2.w;
			p3.z = temp_vec3.z / temp_vec3.w;
</code></pre>
3) Adding the HALF_WIDTH and HALF_HIEGHT to every point's x and y coordinats accordingly, to move the object to the center of the window.
<pre><code>
			p1.x += HALF_WIDTH;
			p2.x += HALF_WIDTH;
			p3.x += HALF_WIDTH;

			p1.y += HALF_HEIGHT;
			p2.y += HALF_HEIGHT;
			p3.y += HALF_HEIGHT;
</code></pre>
<h2>5) The diffrence between translating in the model frame and then rotating in world VS translating in world and then rotating in local:  </h2>
<h4>Screenshot of  translating in the model frame and then rotating in world </h4>
<p>Local:</p>
<pre><code>
scale x = scale y = scale z = 29 //to see the object in a good size
translate x = -375
translate y = -105
</pre></code>
<p>World:</p>
<pre><code>
rotate x = -24
rotate y = -24
rotate z = 24
</pre></code>
<img width="644" alt="5-1" src="https://user-images.githubusercontent.com/75030682/204106775-7a1518a1-40dd-4324-9cc7-515f9cf6fcbf.png">
<h4>Screenshot of  translating in world and then rotating in local </h4>
<p>Local:</p>
<pre><code>
scale x = scale y = scale z = 29 //to see the object in a good size
rotate x = -24
rotate y = -24
rotate z = 24
</code></pre>
<p>World:</p>
<pre><code>
translate x = -375
translate y = -105
</code></pre>
<img width="644" alt="5-2" src="https://user-images.githubusercontent.com/75030682/204106782-53aa6e98-0031-493b-b50c-91ed824fe482.png">
<h5>Another example</h5>
local:  </h2>
<h4>Screenshot of  translating in the model frame and then rotating in world </h4>
<p>Local:</p>
<pre><code>
scale x = scale y = scale z = 29 //to see the object in a good size
translate x = -270
translate y = -165
</pre></code>
<p>World:</p>
<pre><code>
rotate x = 176
rotate y = 108
rotate z = 194
</pre></code>
<img width="644" alt="5-11" src="https://user-images.githubusercontent.com/75030682/204107023-195dbe91-fb9a-4060-97ed-cb2cfd18c539.png">
<h4>Screenshot of  translating in world and then rotating in local </h4>
<p>Local:</p>
<pre><code>
scale x = scale y = scale z = 29 //to see the object in a good size
rotate x = 176
rotate y = 108
rotate z = 194
</code></pre>
<p>World:</p>
<pre><code>
translate x = -270
translate y = -165
</code></pre>
<img width="644" alt="5-22" src="https://user-images.githubusercontent.com/75030682/204107035-faf6bb56-5439-4167-83b4-9a6ba7111100.png">
<h2>6) Only for pairs _ I'm alone  </h2>
<h2>7) The approaches: </h2>
<h3>Keyboard approaches and shortcuts:</h3>
<h4>Translating the object using ASDW:</h4>
<p>When A is pressed: it will move the object to the left</p>
<pre><code>

		if (io.KeysDown[65])//A to translet left
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
			if (scene.GetModelCount() > 0)
			{
				MeshModel curr_mesh = scene.GetActiveModel();
				if (world_or_local == false)
				{
					scene.GetActiveModel().Set_Local_translate_mat(curr_mesh.Get_localX_translate() - 15, curr_mesh.Get_localY_translate(), curr_mesh.Get_localZ_translate());
				}
				else
				{
					scene.GetActiveModel().Set_World_translate_mat(curr_mesh.Get_worldX_translate() - 15, curr_mesh.Get_worldY_translate(), curr_mesh.Get_worldZ_translate());
				}
			}
		}
</code></pre>
<p>When D is pressed: it will move the object to the right</p>
<p>When S is pressed: it will move the object to the down</p>
<p>When W is pressed: it will move the object to the up</p>
<code><pre>
    ******NOTE:
        note that the change is in units of 15.
</code></pre>

<h4>When pressing E and Q:</h4>
<p>When pressing E, the scale of X,Y and Z is up in 1 unit each.</p>
<p>When pressing Q, the scale of X,Y and Z is down in 1 unit each.</p>
<code><pre>
    ******NOTE:
        note that the change is in units of 1.
</code></pre>
<h4>When pressing R and T</h4>
<p>When pressing R, the rotation of Y is up in 1 unit each.</p>
<p>When pressing T, the rotation of Y is down in 1 unit each.</p>
<h4>When pressing F and G</h4>
<p>When pressing F, the rotation of Z is up in 1 unit each.</p>
<p>When pressing G, the rotation of Z is down in 1 unit each.</p>
<h4>When pressing Z and X</h4>
<p>When pressing Z, the rotation of X is up in 1 unit each.</p>
<p>When pressing X, the rotation of X is down in 1 unit each.</p>

<h4>When pressing SPACE</h4>
<p>When pressing SPACE, the controls action is moving from the local to the world, or in the opposite way.</p>


<h2>Video that showing how the shortcuts works:</h2>
<br>
<iframe width="560" height="315" src="https://youtu.be/qEl_mO5WnWI" frameborder="0" allowfullscreen></iframe>
</iframe>
