<h2 style="background-color:DodgerBlue;">1) //My implementation of the algorithm </h2>
<pre><code>
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
</code></pre>
<p>
    <h2 style="background-color:DodgerBlue;">2) Picture of the sanity check results:</h2>
    <img width="137" alt="sanity check1" src="https://user-images.githubusercontent.com/75030682/200136965-040e598f-cc94-46c9-97ba-14aa117d7b97.png">
    <h3>Another picture of the sanity check but with better quality (screenshot)</h3>
    <img width="960" alt="sanity screenshot" src="https://user-images.githubusercontent.com/75030682/200137584-3b80c217-c7cc-44c4-8708-f98e733001a4.png">
</p>
<p>
    <img width="960" alt="sanity screenshot" src="https://user-images.githubusercontent.com/75030682/200137584-3b80c217-c7cc-44c4-8708-f98e733001a4.png">
    <h2 style="background-color:DodgerBlue;">3)A drawing of my own choice:</h2>
    <img width="233" alt="My drawing" src="https://user-images.githubusercontent.com/75030682/200137031-49884cfd-7d89-4687-9e05-885906c30cc8.png">
</p>