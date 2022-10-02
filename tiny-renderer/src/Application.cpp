#include "tgaimage.h"
#include "Model.h"
#include "Geometry.h"
#define IMAGE_WIDTH 800			
#define IMAGE_HEIGHT 800

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

void DrawLine(Vector2i p0, Vector2i p1, TGAImage& image, const TGAColor& color) {
	bool steep = false;
	if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y))
	{
		steep = true;
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
	}
	if (p0.x > p1.x)
	{
		std::swap(p0.x, p1.x);
		std::swap(p1.y, p1.y);
	}

	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = p0.y;

	if (steep)
	{
		for (int x = p0.x; x <= p1.x; x++) {
			image.set_pixel_color(y, x, color);

			error2 += derror2;
			if (error2 > dx)
			{
				y += (p1.y > p0.y ? 1 : -1);
				error2 -= (dx * 2);
			}
		}
	}
	else
	{
		for (int x = p0.x; x <= p1.x; x++) {
			image.set_pixel_color(x, y, color);

			error2 += derror2;
			if (error2 > dx)
			{
				y += (p1.y > p0.y ? 1 : -1);
				error2 -= (dx * 2);
			}
		}
	}
}

void DrawTriangle(Vector2i p0, Vector2i p1, Vector2i p2, TGAImage& image, const TGAColor& color)
{
	//sort vertices in ascending y order
	if (p0.y > p1.y) std::swap(p0, p1);
	if (p0.y > p2.y) std::swap(p0, p2);
	if (p1.y > p2.y) std::swap(p1, p2);
	 
	int totalHeight = p2.y - p0.y;

	for(int y = 0; y <= totalHeight; y++)
	{
		bool isSecondHalf = y > p1.y - p0.y || p1.y == p0.y;
		
		int segmentHeight = isSecondHalf ? p2.y - p1.y : p1.y - p0.y;
		float alpha = (float)y / totalHeight;
		float beta = (float)(y - (isSecondHalf ? p1.y - p0.y : 0)) / segmentHeight;
	
		Vector2i _p0 = p0 + (p2 - p0) * alpha;
		Vector2i _p1 = isSecondHalf ? p1 + (p2 - p1) * beta : p0 + (p1 - p0) * beta;
		
		if (_p0.x > _p1.x)
			std::swap(_p0, _p1);
		for (size_t i = _p0.x; i < _p1.x; i++)
			image.set_pixel_color(i, p0.y+y, color);
	}
}

int main() 
{
	Vector2i t0[3] = { Vector2i(50, 50), Vector2i(200, 100), Vector2i(100, 600)};
	Vector2i t1[3] = { Vector2i(700, 100), Vector2i(500, 623), Vector2i(699, 500)};
	Vector2i t2[3] = { Vector2i(400, 400), Vector2i(500, 320), Vector2i(399, 600)};
	TGAImage image(IMAGE_WIDTH, IMAGE_HEIGHT, TGAImage::RGB);

	DrawTriangle(t0[0], t0[1], t0[2], image, red);
	DrawTriangle(t2[0], t2[1], t2[2], image, green);
	DrawTriangle(t1[0], t1[1], t1[2], image, white);

	image.flip_vertically(); // to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
}

