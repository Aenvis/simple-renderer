#include <iostream>
#include "tgaimage.h"
#include "Model.h"
#include "Geometry.h"
#define SCREEN_WIDTH 800			
#define SCREEN_HEIGHT 500

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

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

void rasterize(Vector2i p0, Vector2i p1, TGAImage& image, const TGAColor& color, int* ybuffer)
{
	if (p0.x > p1.x) std::swap(p0, p1);

	for (int x = p0.x; x < p1.x; x++)
	{
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y * (1.0f - t) + p1.y * t;
		if (y > ybuffer[x])
		{
			ybuffer[x] = y;
			for(size_t i = 0; i < SCREEN_HEIGHT; i++)
			image.set_pixel_color(x, i, color);
		}
	}
}

int main()
{
	TGAImage render(SCREEN_WIDTH, 16, TGAImage::RGB);

	int ybuffer[SCREEN_WIDTH];
	for (size_t i = 0; i < SCREEN_WIDTH; i++)
	{
		ybuffer[i] = INT_MIN;
	}

	rasterize({20, 34}, {744, 400}, render, red, ybuffer);
	rasterize({120, 434}, {444, 400}, render, green, ybuffer);
	rasterize({330, 463}, {594, 200}, render, blue, ybuffer);

	render.flip_vertically(); // to have the origin at the left bottom corner of the image
	render.write_tga_file("output.tga");
}

