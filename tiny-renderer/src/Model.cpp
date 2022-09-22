#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Model.h"

Model::Model(const char* filename) : m_vertices(), m_faces() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
    {
        std::cout << "Could not load " << filename << std::endl;
        return;
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vector3f v;
            for (int i = 0; i < 3; i++) iss >> v.raw[i];
            m_vertices.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            m_faces.push_back(f);
        }
    }
    std::cerr << "# v# " << m_vertices.size() << " f# " << m_faces.size() << std::endl;
}

Model::~Model() {
}

int Model::numberOfVertices() {
    return (int)m_vertices.size();
}

int Model::numberOfFaces() {
    return (int)m_faces.size();
}

std::vector<int> Model::GetFace(int idx) {
    return m_faces[idx];
}

Vector3f Model::GetFaceVertices(int i) {
    return m_vertices[i];
}

