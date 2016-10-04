#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>


ObjLoader::ObjLoader() {
}


ObjLoader::~ObjLoader() {
}


BoilerPlate::Properties::EntityProperties ObjLoader::readObjProp(const char * filePath, int offset) {
	std::ifstream fileStream(filePath);
	std::vector<glm::vec3> vertex = std::vector<glm::vec3>();
	std::vector<unsigned int> index = std::vector<unsigned int>();

	while (!fileStream.eof()) {
		std::string nextChar;
		fileStream >> nextChar;


		if (nextChar == "v") {

			float x, y, z;
			fileStream >> x >> y >> z;
			glm::vec3 newVertex(x, y, z);
			vertex.push_back(newVertex);

		}

		else if (nextChar == "f") {
			unsigned int x, y, z;
			fileStream >> x >> y >> z;

			index.push_back(x);
			index.push_back(y);
			index.push_back(z);
		}




	}
	float *vertexData;
	unsigned int *indexData;
	vertexData = new float[vertex.size() * 3];
	indexData = new unsigned int[index.size()];

	int j = -1;
	for (unsigned int i = 0; i < vertex.size(); i++) {
		vertexData[++j] = vertex.at(i).x;
		vertexData[++j] = vertex.at(i).y;
		vertexData[++j] = vertex.at(i).z;
	}


	for (unsigned int i = 0; i < index.size(); i++) {
		indexData[i] = ((unsigned int)index.at(i)) - (unsigned int)offset;
	}

	BoilerPlate::Properties::EntityProperties newEntityProps { vertexData, indexData, vertex.size() * 3, index.size() };

	return newEntityProps;
}

BoilerPlate::Properties::EntityProperties ObjLoader::readObjProp2(const char * filePath, int offset) {
	std::ifstream fileStream(filePath);
	std::vector<glm::vec3> vertex = std::vector<glm::vec3>();
	std::vector<glm::vec3> normal = std::vector<glm::vec3>();
	std::vector<unsigned int> index = std::vector<unsigned int>();
	std::vector<unsigned int> normalIndex = std::vector<unsigned int>();

	while (!fileStream.eof()) {
		std::string nextChar;
		fileStream >> nextChar;


		if (nextChar == "v") {

			float x, y, z;
			fileStream >> x >> y >> z;
			glm::vec3 newVertex(x, y, z);
			vertex.push_back(newVertex);

		}
		if (nextChar == "vn") {

			float x, y, z;
			fileStream >> x >> y >> z;
			glm::vec3 newVertex(x, y, z);
			normal.push_back(newVertex);

		}

		else if (nextChar == "f") {
			int x, y, z;
			int a, b, c;
			char nothing;
			fileStream >> x;
			fileStream >> nothing;
			fileStream >> nothing;
			fileStream >> a;
			fileStream >> y >> nothing >> nothing >> b
			>> z >> nothing >> nothing >> c;

			index.push_back(x);
			index.push_back(y);
			index.push_back(z);
			normalIndex.push_back(a);
			normalIndex.push_back(b);
			normalIndex.push_back(c);

		}

		




	}

	float *vertexData, *normalData, *normalData2;
	unsigned int *indexData;
	vertexData = new float[vertex.size() * 3];
	normalData = new float[vertex.size() * 3];
	normalData2 = new float[vertex.size() * 3];

	indexData = new unsigned int[index.size()];

	int j = -1;
	for (unsigned int i = 0; i < vertex.size(); i++) {
		vertexData[++j] = vertex.at(i).x;
		vertexData[++j] = vertex.at(i).y;
		vertexData[++j] = vertex.at(i).z;
	}
	 j = -1;
	for (unsigned int i = 0; i < normal.size(); i++) {
		normalData[++j] = normal.at(i).x;
		normalData[++j] = normal.at(i).y;
		normalData[++j] = normal.at(i).z;
	}


	for (unsigned int i = 0; i < index.size(); i++) {
		indexData[i] = ((unsigned int)index.at(i)) - (unsigned int)offset;
		/*
		unsigned int indexVal = indexData[i];
		unsigned int normalVal = (unsigned int) normalIndex.at(i) - (unsigned int)offset;
		normalData2[indexVal] = normalData[normalVal];
		*/
	}
	for (unsigned int i = 0; i < index.size(); i+=3) {
		GLushort ia = indexData[i];
		GLushort ib = indexData[i + 1];
		GLushort ic = indexData[i + 2];
		glm::vec3 thisNormal = glm::normalize(glm::cross(
		glm::vec3(vertex[ib]) - glm::vec3(vertex[ia]),
		glm::vec3(vertex[ic]) - glm::vec3(vertex[ia])));
		normal[ia] = normal[ib] = normal[ic] = thisNormal;
	}
	j = -1;
	for (unsigned int i = 0; i < vertex.size(); i++) {
		normalData2[++j] = normal.at(i).x;
		normalData2[++j] = normal.at(i).y;
		normalData2[++j] = normal.at(i).z;
	}

	std::ofstream out;
	out.open("./Scissors.cpp");
	out << "unsigned int vertexCount = " << (vertex.size() * 3) << ";\n";
	out << "unsigned int indexCount = " << index.size() << ";\n";
	out << "float sphereVertices[] = {\n";

	for (unsigned int i = 0; i < vertex.size() * 3; i++) {
		char endChar = (i == (vertex.size() * 3) - 1) ? ' ' : ',';
		out << vertexData[i] << endChar << '\n';
	}

	out << "};\n";
	out << "float scissorNormals[] = {\n";
	j = -1;
	for (unsigned int i = 0; i < vertex.size() * 3; i++) {
		char endChar = (i == (vertex.size() * 3 ) - 1) ? ' ' : ',';
		out << normalData2[i] << endChar << '\n';
	}
	out << "};\n";
	out << "unsigned int scissorIndices[] = {\n";
	for (unsigned int i = 0; i < index.size(); i++) {
		char endChar = (i == index.size() - 1) ? ' ' : ',';
		out << indexData[i] << endChar << '\n';
	}
	out << "};\n";
	out.close();
	

	BoilerPlate::Properties::EntityProperties newEntityProps;
	newEntityProps.indexSize = index.size();
	newEntityProps.vertexSize = vertex.size();
	newEntityProps.indices = indexData;
	newEntityProps.vertices = vertexData;
	newEntityProps.normals = normalData2;

	return newEntityProps;
}

BoilerPlate::Properties::EntityProperties ObjLoader::readObjProp3(const char * filePath, int offset) {
	std::ifstream fileStream(filePath);
	std::vector<glm::vec3> vertex = std::vector<glm::vec3>();
	std::vector<glm::vec3> normal = std::vector<glm::vec3>();
	std::vector<glm::vec2> texture = std::vector<glm::vec2>();
	std::vector<unsigned int> index = std::vector<unsigned int>();
	std::vector<unsigned int> normalIndex = std::vector<unsigned int>();
	std::vector<unsigned int> textureIndex = std::vector<unsigned int>();
	bool vExist = false, vnExist = false, vtExist = false, fExist = false;

	while (!fileStream.eof()) {
		std::string nextChar;
		fileStream >> nextChar;


		if (nextChar == "v") {
			if (!vExist)
				vExist = true;
			float x, y, z;
			fileStream >> x >> y >> z;
			glm::vec3 newVertex(x, y, z);
			vertex.push_back(newVertex);

		}
		if (nextChar == "vn") {
			if (!vnExist)
				vnExist = true;
			float x, y, z;
			fileStream >> x >> y >> z;
			glm::vec3 newVertex(x, y, z);
			normal.push_back(newVertex);

		}

		if (nextChar == "vt") {
			if (!vtExist)
				vtExist = true;
			float u, v;
			fileStream >> u >> v;
			glm::vec2 ntexture(u, v);
			texture.push_back(ntexture);

		}

		else if (nextChar == "f") {
			int x, y, z;
			int a, b, c;
			int u, v, w;
			char nothing;
			if(vExist)
				fileStream >> x;
			if(vtExist || vnExist)
				fileStream >> nothing;
			if (vtExist)
				fileStream >> u;
			if (vtExist || vnExist)
				fileStream >> nothing;
			if(vnExist)
				fileStream >> a;
			if (vExist)
				fileStream >> y;
			if (vtExist || vnExist)
				fileStream >> nothing;
			if (vtExist)
				fileStream >> v;
			if (vtExist || vnExist)
				fileStream >> nothing;
			if (vnExist)
				fileStream >> b;
			if (vExist)
				fileStream >> z;
			if (vtExist || vnExist)
				fileStream >> nothing;
			if (vtExist)
				fileStream >> w;
			if (vtExist || vnExist)
				fileStream >> nothing;
			if (vnExist)
				fileStream >> c;

			if (vExist) {
				index.push_back(x);
				index.push_back(y);
				index.push_back(z);
			}
			if (vnExist) {
				normalIndex.push_back(a);
				normalIndex.push_back(b);
				normalIndex.push_back(c);
			}
			if (vtExist) {
				textureIndex.push_back(u);
				textureIndex.push_back(v);
				textureIndex.push_back(w);
			}
		}

	}

	float *vertexData, *normalData, *textureData;
	unsigned int *indexData;
	vertexData = new float[vertex.size() * 3];
	normalData = new float[vertex.size() * 3];
	textureData = new float[texture.size() * 3];

	indexData = new unsigned int[index.size()];

	for (unsigned int i = 0; i < index.size(); i++) {
		indexData[i] = ((unsigned int)index.at(i)) - (unsigned int)offset;
	}
	for (unsigned int i = 0; i < normalIndex.size(); i++) {
		normalIndex[i] = ((unsigned int)normalIndex.at(i)) - (unsigned int)offset;
	}
	for (unsigned int i = 0; i < textureIndex.size(); i++) {
		textureIndex[i] = ((unsigned int)textureIndex.at(i)) - (unsigned int)offset;
	}

	int j = -1;
	for (unsigned int i = 0; i < vertex.size(); i++) {
		int holder = j;
		vertexData[++j] = vertex.at(i).x;
		vertexData[++j] = vertex.at(i).y;
		vertexData[++j] = vertex.at(i).z;
		j = holder;
		
	}

	j = -1;
	for (unsigned int i = 0; i < normal.size(); i++) {
		j++;
		normalData[indexData[j]] = normal.at(normalIndex[i]).x;
		j++;
		normalData[indexData[j]] = normal.at(normalIndex[i]).y;
		j++;
		normalData[indexData[j]] = normal.at(normalIndex[i]).z;
	}




	BoilerPlate::Properties::EntityProperties newEntityProps;
	newEntityProps.indexSize = index.size();
	newEntityProps.vertexSize = vertex.size();
	newEntityProps.indices = indexData;
	newEntityProps.vertices = vertexData;
	newEntityProps.normals = normalData;

	return newEntityProps;

}


