#pragma once
#include <vector>
#include "Geometry.h"

class Model {
private:
	std::vector<Vector3f> m_vertices;
	std::vector<std::vector<int> > m_faces;
public:
	Model(const char* fileName);
	~Model();
	int numberOfVertices();
	int numberOfFaces();
	Vector3f GetFaceVertices(int id);
	std::vector<int> GetFace(int id);
};