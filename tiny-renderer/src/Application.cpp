#include <iostream>
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

Vector3f barycentricCoordinates(Vector2i* points, Vector2i p)
{
	Vector3f a(points[2].x - points[0].x, points[1].x - points[0].x, points[0].x - p.x);
	Vector3f b(points[2].y - points[0].y, points[1].y - points[0].y, points[0].y - p.y);
	Vector3f u = a ^ b;

	if (std::abs(u.z) < 1) return Vector3f(-1.0f, 1.0f, 1.0f);
	return Vector3f(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void DrawTriangle(Vector2i* points, TGAImage& image, const TGAColor& color)
{
	Vector2i boundingBoxMin(image.get_width() - 1, image.get_height() - 1);
	Vector2i boundingBoxMax(0, 0);
	Vector2i clamp(image.get_width() - 1, image.get_height() - 1);

	for (size_t i = 0; i < 3; i++)
	{
		boundingBoxMin.x = std::max(0, std::min(boundingBoxMin.x, points[i].x));
		boundingBoxMin.y = std::max(0, std::min(boundingBoxMin.y, points[i].y));

		boundingBoxMax.x = std::min(clamp.x, std::max(boundingBoxMax.x, points[i].x));
		boundingBoxMax.y = std::min(clamp.y, std::max(boundingBoxMax.y, points[i].y));
	}
	Vector2i p;
	for (p.x = boundingBoxMin.x; p.x <= boundingBoxMax.x; p.x++)
	{
		for (p.y = boundingBoxMin.y; p.y <= boundingBoxMax.y; p.y++)
		{
			Vector3f barycentricOnScreen = barycentricCoordinates(points, p);
			if (barycentricOnScreen.x < 0 || barycentricOnScreen.y < 0 || barycentricOnScreen.z < 0) continue;
			image.set_pixel_color(p.x, p.y, color);
		}
	}
}

int main() 
{
	Model* model = new Model("models/model.obj");
	Vector3f lightDir(0.f, 0.f, -1.f); 
	TGAImage image(IMAGE_WIDTH, IMAGE_HEIGHT, TGAImage::RGB);

	for (size_t i = 0; i < model->numberOfFaces(); i++)
	{
		std::vector<int> face = model->GetFace(i);
		Vector2i screenCoordinates[3];
		Vector3f worldCoordinates[3];
		for (size_t j = 0; j < 3; j++)
		{
			worldCoordinates[j] = model->GetFaceVertices(face[j]);
			screenCoordinates[j] = Vector2i((worldCoordinates[j].x + 1.0f) * IMAGE_WIDTH / 2.0f, (worldCoordinates[j].y + 1.0f) * IMAGE_HEIGHT / 2.0f);
		}
		Vector3f normalVector = Vector3f(worldCoordinates[2] - worldCoordinates[0]) ^ Vector3f(worldCoordinates[1] - worldCoordinates[0]);
		normalVector.normalize();

		float intensity = normalVector * lightDir;

		if(intensity > 0) DrawTriangle(screenCoordinates, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
	}

	image.flip_vertically(); // to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
}

