#include "tgaimage.h"
#include "Model.h"
#include "Geometry.h"
#define IMAGE_WIDTH 800			
#define IMAGE_HEIGHT 800

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

struct Vector2
{
	int x = 0, y = 0;

	Vector2(int x,int y)
		: x(x), y(y) {}
};

void DrawLine(Vector2 p0, Vector2 p1, TGAImage& image, const TGAColor& color) {
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
int main() {
	Model* model = new Model("obj/model.obj");

	TGAImage image(IMAGE_WIDTH, IMAGE_HEIGHT, TGAImage::RGB);

	for (size_t i = 0; i < model->numberOfFaces(); i++)
	{
		std::vector<int> face = model->GetFace(i);
		
		for (size_t j = 0; j < 3; j++)
		{
			Vector3f v0 = model->GetFaceVertices(face[j]);
			Vector3f v1 = model->GetFaceVertices(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.0f) * IMAGE_WIDTH / 2;
			int y0 = (v0.y + 1.0f) * IMAGE_HEIGHT / 2;
			int x1 = (v1.x + 1.0f) * IMAGE_WIDTH / 2;
			int y1 = (v1.y + 1.0f) * IMAGE_HEIGHT / 2;
			DrawLine({ x0, y0 }, { x1, y1 }, image, white);
		}
	}
	image.flip_vertically(); // to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	delete model;
}

