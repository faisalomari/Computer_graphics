<h1 style="color:Red;">Assignment 1_c report</h1>
<h2 style="color:DodgerBlue;">from: Faisal Omari, ID: 325616894</h2>
<h3 style="color:Tomato;">1)</h3>
<img width="639" alt="Ac_11" src="https://user-images.githubusercontent.com/75030682/208264333-5c3cba00-fcca-4d34-913d-799287de9ee7.png">
as shown in the image in the left up side we can see the created window that gives the user to control up,down,left,right,near,far with a sliders.
<h3 style="color:Tomato;">2)</h3>
<img width="239" alt="Ac_21" src="https://user-images.githubusercontent.com/75030682/208264380-c982fae7-8b69-499a-a1da-b995172493e5.png">
as shown in the image I gave the user to reposition the camera using sliders to control the translate and rotation of the three axes x,y and z, the matrices and fuunction were implemented as in the previous assignment with the same transformation matrices but only without scaling because there is no scaling required for the camera...
<h3 style="color:Tomato;">3)</h3>
What I've done in theis branch is to complete the TODO function in the main.cpp which updates the width and height according to the changed window coordinats...
<p>First Image that shows the window in small mode<p>
<img width="239" alt="Ac_21" src="https://user-images.githubusercontent.com/75030682/208264380-c982fae7-8b69-499a-a1da-b995172493e5.png">
<p>Second Image that shows the window in almost full resoultion mode</p><img width="960" alt="Ac_32" src="https://user-images.githubusercontent.com/75030682/208264489-c17619b2-2108-4369-bd0e-a5024e88586b.png">
<p>and as shown that obvilously the resolution of the loaded image is scaled accordingly to the changed parameters of the window, that is done in the file renderer.cpp in the function DrawMesh, and there I've calculated the vectors again and normalized them as I've done in the previous assignment, and after that to make the whole story completed, I added the following sections which calculate the new vectors x and y values by multiplying them with the new collected width and height, and that gives us the required scale to do the mission as required. </p>
<h5>The code section:</h5>
<code>
px_max.x = (px_max.x + 1) * width;
px_max.y = (px_max.y + 1) * height;
py_max.x = (py_max.x + 1) * width;;
py_max.y = (py_max.y + 1) * height;
pz_max.x = (pz_max.x + 1) * width;;
pz_max.y = (pz_max.y + 1) * height;
center.x = (center.x + 1) * width;;
center.y = (center.y + 1) * height;
</code>
<h3 style="color:Tomato;">4) </h3>
<p>Ss shown in the following video, that firstly we translate and rotate in local, and after that we do the same but in world mode, and that the axes is shown and the object axes is moving with it accordingly to the rotations and the translations that were been done. </p>
<h5>A video that shows a run test that checks and shows how it is done, and what is the difference:</h5>
<video width="100" height="100" controls>
  <source src="https://user-images.githubusercontent.com/75030682/208265165-2c011752-5313-41f3-8aa8-f5e8313df2b4.mp4
" type="video/mp4">
Your browser does not support the video tag.
</video>
https://user-images.githubusercontent.com/75030682/208265165-2c011752-5313-41f3-8aa8-f5e8313df2b4.mp4
<h3 style="color:Tomato;">5) </h3>
The used T1 and T2 is as following:
<code>
T1:
translate_X = 45
translate_Y = 30
rotate_x = 200
rotate_y = 230
rotate_z =150
T2:
translate_X = -45 
translate_Y = -45
rotate_x = 79
rotate_y = 54
rotate_z = 103
</code>
and the results were as follow:
<p>when T1 applied to local and T2 applied to world</p>
<img width="639" alt="5_1c" src="https://user-images.githubusercontent.com/75030682/208264548-be192380-2513-4d66-824c-053dcaa56189.png">
<p>when T2 applied to local and T1 applied to world</p>
<img width="638" alt="5_2c" src="https://user-images.githubusercontent.com/75030682/208264554-b60268c0-83a0-4aba-81e1-a04565515f3e.png">
<h3 style="color:Tomato;">6)</h3>
<p>The bounding box as described in the axes was calculated using the min and max values of the axes x and y and z, then accordingly the faces of the box were drawed and displayd as the bounding box, example:</p>
<img width="638" alt="boundingbox" src="https://user-images.githubusercontent.com/75030682/208264592-5eca260d-958e-4985-be1f-71194c46bd2c.png">
<h3 style="color:Tomato;">7)</h3>
<h4>The face normals is as following:</h4>
<p>For every face I calculated the normal using the formula we learned, after that I scaled the normal accordingly in a lot of tests and accordingly I chose the best scale value, then I ecalculated the center of the face and from this point I Drew a line in the normal direction</p>
<h5>an image as example:</h5>
<img width="639" alt="faces_normals" src="https://user-images.githubusercontent.com/75030682/208265462-f53e2bc4-06d1-4932-a643-fef77918c71f.png">
<h4>The vertices normals is as following:</h4>
<p>The vertices normals were calculated normals , I scaled the value and desplayed them in the screen accordingly, I also used some skills from the Faces calculation process.</p>
<h5>an image as example:</h5>
<img width="638" alt="vertices_normals" src="https://user-images.githubusercontent.com/75030682/208265440-42dd3ad9-e0e4-41ff-86c4-21e15b485410.png">
<h3 style="color:Tomato;">8)</h3>
<p>As shown in the following images, when we translate z in perspective mode, the camera is doing like a zoom to the loaded object(which is chain in these photos), and when we put the system in orthogonal mode, the camera is not scaling or moving away or closer to the object, and nothing is changed.</p>
<h5>In the following image the chain.obj is loaded , it is in orthogonal, and the translate z value is setted to 1 (not changed)</h5>
<img width="638" alt="orth_1" src="https://user-images.githubusercontent.com/75030682/208265123-0ea8e4a2-b4c8-479f-98ed-9dd27f05ec15.png">
<h5>In the following image the chain.obj is loaded , it is in orthogonal, and the translate z value is setted to 640 (The maximum), and it is obvious that nothing is changed</h5>
<img width="638" alt="orth_2" src="https://user-images.githubusercontent.com/75030682/208265125-54563336-9783-4c99-861d-cf23364149e7.png">
<h5>In the following image the chain.obj is loaded , it is in orthogonal, and the translate z value is setted to -640 (The minimum), and it is obvious that nothing is changed</h5>
<img width="638" alt="orth_3" src="https://user-images.githubusercontent.com/75030682/208265126-5d49796d-cf24-409b-a290-972b24b5f65e.png">
<h5>In the following image the chain.obj is loaded , it is in perspective, and the translate z value is setted to 240 (not the max), and it is obvious that it is very scaled on the object</h5>
<img width="639" alt="pers_1" src="https://user-images.githubusercontent.com/75030682/208265127-abe835d9-d534-471f-8679-233e0eb9a316.png">
<h5>In the following image the chain.obj is loaded , it is in perspective, and the translate z value is setted to 640 (the max), and it is obvious that it is not scaled on the object, and its looks natural as needed</h5>
<img width="638" alt="pers_2" src="https://user-images.githubusercontent.com/75030682/208265129-bb7779d2-a7d6-450d-8c0c-ee626de13ec8.png">
<p>And as we know that this is what is required to be when applying this settings, the main reason for this to be working is the following code section, which is the main change between the orthogonal and perspective modes:</p>
<code>
	glm::mat4x4 lookat = glm::lookAt(eye, at, up);
	glm::mat4x4 ortho;
	glm::mat4x4 pers;
	if (flag_Orth_pro) {
		ortho = glm::ortho(left, right, down, top, near_1, far_1);
		return ortho * lookat * inversemat;
	}
	else {
		pers = glm::perspective(fovy, asp, near_2, far_2);
		return pers * lookat * inversemat;
	}
</code>
<p>This code is checking the flag_Orth_pro which is lighting if the mode is orthogonal, and 0 if it is perspective, and we are using the ortho and perspective functions from the glm library that completes the calculation accordingly.</p>
<h3 style="color:Tomato;">13)</h3>
<p>An image that displays the whole GUI in the end of the assignment:</p>
<p>we can see the three created windows, one to control the local variables, and another to controls the world's</p>
<p>and the one that we created in this assignment which controls the camera variables and valus accoridngly... </p>
<h4>The Image:</h4>
<img width="922" alt="GUISC" src="https://user-images.githubusercontent.com/75030682/208264715-64529f81-ca1b-4be5-a21c-2d9f3ba1b4c0.png">
<h3 style="color:Tomato;">14)</h3>
<p>The additional features that I've created in this assignment is as follow:</p>
<h5>1)Window that shows the controls of the camera</h5>
<h5>2)A check box to control weither you want the mesh to be displayed in the orthogonal or perspetive.</h5>
<h5>3)A check boxes that asks the user to choose what they want to be displayed in the GUI, such as local axes, faces normals, vertices normals, bounding box.</h5>
<h5>4)An option to undisplay the world axes that is fixed according to the camera's world translation, when space is tabed, the axes will be undeplayed, and when it is tabed again it will be displayed, and so on....</h5>