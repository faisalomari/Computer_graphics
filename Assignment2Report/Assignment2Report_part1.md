<h1>Assignment 2_a Report</h1>
<h2>1) Drawing the triangle bounding box instead of rectangel from the previous assignment, each new box with unique color, In addition to adding a checkbox that gives the user the opition to choose wether he want's to draw these boxes or not</h2>
<p>The rectangle is drawn using the method from the previous assignment such that we get the min and max of x-axis, y-axis and z-axis, and accordingly we calculate the rectangle lines and drawing it in a color that is related to the depth.</p>
<p>An image showing the drawed triangle boxes:</p>
<img src=https://user-images.githubusercontent.com/75030682/211168597-22458f7a-e653-4b29-9388-c5c786b66349.png>

<p>An Image that shows the checkbox option:</p>
<img src=https://user-images.githubusercontent.com/75030682/211168704-53143a0c-7637-4009-befd-c6359bc5d005.png  >

<h2>2)In this branch, what we did was to iterate over all the faces and points of the vertices, and then checking if these points of the vertices are inside the rectangle box, if yes then we'll manipulate the color of the drawn pixel such that we multiply it with a const and the vertices, and finally we check if the direction of the vercotrs is in the same direction then we draw the point and we figure that it is inside the triangle, else we figure out that this point is outside the triangle box and it should not be drawn.</h2>
<h2>3) Here is an image that is showing the branch description applied to the loaded mesh scene:</h2>
<img src=https://user-images.githubusercontent.com/75030682/211168761-0b0a3e9a-e2e4-42df-aec8-c10596b7183b.png  >
<h2>4) Here is a punch of images that shows how the algorithm implementation really do works in the mesh scene with different objects loaded:</h2>
<h3>An image that shows the effect before translating z:<h3>
<img src=https://user-images.githubusercontent.com/75030682/211168770-b117e5cd-6f3c-4899-9581-229c469bf51f.png  >
<h3>An image that shows the effect after translating z:<h3>
<img src=https://user-images.githubusercontent.com/75030682/211168764-318da604-09f9-46a2-aa30-17ea8f3baace.png  >
<h3>An image that shows how the two objects get one inside the other </h3>
<img src=https://user-images.githubusercontent.com/75030682/211168772-a0be7269-2ab9-4837-80c2-e678ac5827ce.png  >
