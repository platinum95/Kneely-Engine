#include "ModelLoader.h"
#include <iostream>
#include "Texture.h"

#include "ImageLoader.h"



static const BoilerPlate::Properties::BufferObjectProperties floatPropsTwoD{
	2,
	sizeof(float),
	GL_FALSE,
	GL_ARRAY_BUFFER,
	GL_STATIC_DRAW,
	GL_FLOAT
};
static const BoilerPlate::Properties::BufferObjectProperties floatProps{
	3,
	sizeof(float),
	GL_FALSE,
	GL_ARRAY_BUFFER,
	GL_STATIC_DRAW,
	GL_FLOAT
};

unsigned int ModelLoader::pFlags;

ModelLoader::ModelLoader() {
}

std::vector<Entity*> *ModelLoader::readModel(const char * filePath, unsigned int _pFlags) {
	std::vector<Entity*> *MeshVec = new std::vector<Entity*>();
	std::vector<BoilerPlate::Properties::EntityProperties> EntPropVec = std::vector<BoilerPlate::Properties::EntityProperties>();
	Assimp::Importer *import = new Assimp::Importer;
	pFlags = _pFlags;
	const struct aiScene* scene = import->ReadFile(filePath, pFlags);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import->GetErrorString() << std::endl;
		return MeshVec;
	}
	std::string directory = std::string(filePath).substr(0, std::string(filePath).find_last_of('/'));
	
	processNode(scene->mRootNode, scene, MeshVec);

	return MeshVec;
}


ModelLoader::~ModelLoader() {
}

Entity *ModelLoader::processMesh(aiMesh inMesh, const aiScene* inScene) {
	BoilerPlate::Properties::EntityProperties newProp;
	Entity* newEnt = new Entity;
	std::vector<Texture*> textures = std::vector<Texture*>();
	newProp.indexSize = inMesh.mNumFaces * 3;

	newProp.indices = new unsigned int[inMesh.mNumFaces * 3];

	int j = -1;
	for (unsigned int i = 0; i < inMesh.mNumFaces; i++) {
		aiFace face = inMesh.mFaces[i];
		
		for(unsigned int k = 0; k < face.mNumIndices; k++)
			newProp.indices[++j] = face.mIndices[k];
	}

	
	if (inMesh.HasPositions()) {
		newProp.vertices = new float[inMesh.mNumVertices * 3];
		int j = -1;
		for (unsigned int i = 0; i < inMesh.mNumVertices; i++) {
			newProp.vertices[++j] = inMesh.mVertices[i].x;
			newProp.vertices[++j] = inMesh.mVertices[i].y;
			newProp.vertices[++j] = inMesh.mVertices[i].z;
		}
	}
	
	if (inMesh.HasNormals()) {
		newProp.normals = new float[inMesh.mNumVertices * 3];
		int j = -1;
		for (unsigned int i = 0; i < inMesh.mNumVertices; i++) {
			newProp.normals[++j] = inMesh.mNormals[i].x;
			newProp.normals[++j] = inMesh.mNormals[i].y;
			newProp.normals[++j] = inMesh.mNormals[i].z;
		}
	}
	
	if (inMesh.HasTextureCoords(0)) {
		float* texCoords = new float[inMesh.mNumVertices * 2];
		int j = -1;
		for (unsigned int i = 0; i < inMesh.mNumVertices; i++) {
			texCoords[++j] = inMesh.mTextureCoords[0][i].x;
			texCoords[++j] = inMesh.mTextureCoords[0][i].y;
		}
		BufferObject *texBO = new BufferObject(floatPropsTwoD, 2, inMesh.mNumVertices * 2, texCoords);
		newEnt->registerBufferObject(texBO);

	}
	if ((pFlags & aiProcess_CalcTangentSpace) && inMesh.HasTangentsAndBitangents()) {
		float* tangents = new float[inMesh.mNumVertices * 3];
		float* bitangents = new float[inMesh.mNumVertices * 3];
		int j = -1;
		for (unsigned int i = 0; i < inMesh.mNumVertices; i++) {
			tangents[++j] = inMesh.mTangents[i].x;
			bitangents[j] = inMesh.mBitangents[i].x;
			tangents[++j] = inMesh.mTangents[i].y;
			bitangents[j] = inMesh.mBitangents[i].y;
			tangents[++j] = inMesh.mTangents[i].z;
			bitangents[j] = inMesh.mBitangents[i].z;
		}
		BufferObject *tangentBO = new BufferObject(floatProps, 3, inMesh.mNumVertices * 3, tangents);
		BufferObject *bitangentBO = new BufferObject(floatProps, 4, inMesh.mNumVertices * 3, bitangents);
		newEnt->registerBufferObject(tangentBO);
		newEnt->registerBufferObject(bitangentBO);
	}
	
	if (inMesh.mMaterialIndex >= 0) {
		aiMaterial* material = inScene->mMaterials[inMesh.mMaterialIndex];
		std::vector<Texture*> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture*> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		for (Texture *t : textures) {
			newEnt->textures.push_back(t);
		}
	}
	
	newProp.indexSize;
	newProp.vertexSize = inMesh.mNumVertices;
	BufferObject* newVert = new BufferObject(floatProps, 0, newProp.vertexSize*3, newProp.vertices);
	BufferObject* newNorms = new BufferObject(floatProps, 1, newProp.vertexSize *3, newProp.normals);
	newEnt->registerBufferObject(newVert);
	newEnt->registerBufferObject(newNorms);
	newEnt->createEntity(newProp.indices, newProp.indexSize);

	return newEnt;
}
void ModelLoader::freeData(Entity *_ent) {
	for (BufferObject* BO : _ent->VBOs) {
		delete[] BO->data;
	//	delete BO;
	}
	delete[] _ent->indices;
}

std::vector<Texture*> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture*> textures = std::vector<Texture*>();
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture *texture;
		ImageData grass = ImageLoader::loadPNG(str.C_Str());
		texture = new Texture(grass, GL_TEXTURE_2D);
		texture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		texture->registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		texture->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		texture->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		texture->textureBank = GL_TEXTURE0;
		texture->updateTexture();
//		ImageLoader::freeData(grass);

		textures.push_back(texture);
	}
	return textures;
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene, std::vector<Entity*> *propList) {
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		propList->push_back(processMesh(*mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, propList);
	}
}