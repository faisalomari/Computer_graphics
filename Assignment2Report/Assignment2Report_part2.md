<h1>Assignment 2b (part 2) ___ Report</h1>
<img src=>
<h2>1)</h2>
<p>The ambient light has been calculated according to the formula as we learned in the class, The result we got is</p>
<code>RGB = (0.3243,0.096,0.1424)</code>
<p>from  using the two color as following:</p>
<code>Light: RGB=(0.47,0.6,0.89)</code>
<code>Object: RGB=(0.69,0.16,0.16)</code>
and because of the natural of the ambient lighhts that it is not releated to the main light position, there was no change at all of the mabient light when we changed the location of the main light.
<p>Two images that shows the results:</p>
<p>An Image that shows the checkbox option:</p>
<img src=https://user-images.githubusercontent.com/75030682/215286924-78384ba4-1468-4ae2-9d22-6944f9981a31.png>
<img src=https://user-images.githubusercontent.com/75030682/215286968-8c402c4d-4474-4b8c-a406-35ca81218f60.png>

<h2>2)</h2>
<p>The diffuse light was calculated using the formula presented in the lecture: Id = Kd * (l . dot (n)) * Ld.</p>
<p>An image that shows the results:</p>
<img src=https://user-images.githubusercontent.com/75030682/215286969-6fe5b12e-4db2-4af6-a314-18340e8f3012.png>
<h2>4)</h2>
<p>We calculated the reflected light for each face using the formula "r = 2*n * glm::dot(n, l) - l" and drew it using yellow lines. For the light ray, we used the "l" vector direction light (lightlocation-point) and drew it in torquise color.</p>
<p>Two images that shows the results:</p>
<img src=https://user-images.githubusercontent.com/75030682/215286971-a0c9de6a-a58f-4512-b237-1a1eff611237.png>
<img src=https://user-images.githubusercontent.com/75030682/215286972-8581d96f-c3d5-42c3-a291-a6a203b05b76.png>
<h2>5)</h2>
<p>We calculated the specular light according to the formula presented in the lecture: "r = 2 * n * glm::dot(n, l) - l". The "v" vector represents the camera's eye vector, and the intensity of the specular light is calculated as "Is = Ks * glm::dot(r,v) * ls", where "Ks" is the specular coefficient and "ls" is the light source intensity.</p>
<p>An image that shows the results:</p>
<img src=https://user-images.githubusercontent.com/75030682/215286973-9cb4cd54-387d-44e3-9e0a-76cbe4465cde.png>
<h2>6)</h2>
<p>The addition above shows a lot of details of the mesh and the implementations, in Addition here is a few images that shows how the light is done on one/two object with double lights:</p>
<img src=https://user-images.githubusercontent.com/75030682/215286975-aaad7ed5-0a54-4768-b8fd-4d0f5cf5437b.png>
<img src=https://user-images.githubusercontent.com/75030682/215286976-0f0caa1b-3fcb-4215-93ba-8c8bffe94fdf.png>
<img src=https://user-images.githubusercontent.com/75030682/215286977-a2e5eb22-2715-44e6-95c9-664b4bcbffd9.png>
<img src=https://user-images.githubusercontent.com/75030682/215286978-9e04544d-87a4-4877-b7b0-e0c3b6f34bd1.png>
<h2>7)</h2>
<p>First texture implementation that we used is Blur,</p>
<p>In the Box Blur implementation, we used convolution with a 5x5 filter. Each pixel in the resulting image has a value equal to the average value of its neighboring pixels.</p>
<p>Three image that shows the results:</p>

<p>Second texture implementation that we used is Fog,</p>
<p>Three image that shows the results:</p>
<p>Blured Banana:</p>
<img src=https://user-images.githubusercontent.com/75030682/215286979-7fcabb72-c439-4d9d-b2d1-9967e7135add.png>
<p>With Fig and Without Fog:</p>
<img src=https://user-images.githubusercontent.com/75030682/215286980-7b5db124-c481-4b56-b7d0-4c664b1adbe2.png>
<img src=https://user-images.githubusercontent.com/75030682/215286981-fed2c33d-af46-4bdb-932d-96869a92e468.png>