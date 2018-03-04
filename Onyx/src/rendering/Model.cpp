#include "Model.hpp"

#include "Shader.hpp"
#include "Camera.hpp"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/ProgressHandler.hpp>

#include "math\Ray.hpp"
#include "math\AABB.hpp"

#include <iostream>
#include <map>

namespace
{
	std::map<std::string, Mesh::Ptr> m_meshMap;
}

void Model::loadFromFile(const std::string& filename)
{
	std::map<std::string, Mesh::Ptr>::const_iterator it = m_meshMap.find(filename);

	if (it != m_meshMap.end())
	{
		m_mesh = it->second;

		std::cout << "\nLoaded: " << filename << " from mesh map" << std::endl;
	}
	else
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals);

		if (!scene)
		{
			std::cout << importer.GetErrorString() << std::endl;
			return;
		}

		m_mesh = Mesh::create();
		
		const aiMesh* model = scene->mMeshes[0];
		
		for (unsigned int i = 0; i < model->mNumVertices; i++)
		{
			const aiVector3D pos = model->mVertices[i];
			const aiVector3D norm = model->mNormals[i];

			Vertex vertex({ Vector3f(pos.x, pos.y, pos.z), Vector3f(norm.x, norm.y, norm.z) });

			m_mesh->addVertex(vertex);
		}

		for (unsigned int i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];

			assert(face.mNumIndices == 3);

			m_mesh->addIndex(face.mIndices[0]);
			m_mesh->addIndex(face.mIndices[1]);
			m_mesh->addIndex(face.mIndices[2]);
		}
	
		m_mesh->complete();

		m_mesh->bind();
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		m_mesh->unbind();

		m_meshMap.insert(std::make_pair(filename, m_mesh));

		std::cout << "\nFinished loading: " << filename << " with " << model->mNumVertices << " vertices..." << std::endl;
	}
}

void Model::saveToFile(const std::string& filename)
{
	Assimp::Exporter exporter;

	aiScene scene;

	scene.mRootNode = new aiNode();

	scene.mMaterials = new aiMaterial*[1];
	scene.mMaterials[0] = nullptr;
	scene.mNumMaterials = 1;

	scene.mMaterials[0] = new aiMaterial();

	scene.mMeshes = new aiMesh*[1];
	scene.mMeshes[0] = nullptr;
	scene.mNumMeshes = 1;

	scene.mMeshes[0] = new aiMesh();
	scene.mMeshes[0]->mMaterialIndex = 0;

	scene.mRootNode->mMeshes = new unsigned int[1];
	scene.mRootNode->mMeshes[0] = 0;
	scene.mRootNode->mNumMeshes = 1;
		
	auto pMesh = scene.mMeshes[0];

	pMesh->mVertices = new aiVector3D[m_mesh->getSize()];
	pMesh->mNumVertices = m_mesh->getSize();
	pMesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;
	pMesh->mNormals = new aiVector3D[m_mesh->getSize()];

	for (size_t i = 0; i < m_mesh->getSize(); ++i)
	{
		Vertex v = m_mesh->operator[](i);

		pMesh->mVertices[i] = aiVector3D(v.position.x, v.position.y, v.position.z);
		pMesh->mNormals[i] = aiVector3D(v.normal.x, v.normal.y, v.normal.z);
	}
		
	pMesh->mFaces = new aiFace[m_mesh->getSize() / 3];
	pMesh->mNumFaces = m_mesh->getSize() / 3;

	for (size_t i = 0; i < m_mesh->getSize() / 3; ++i) 
	{
		aiFace& face = pMesh->mFaces[i];

		face.mIndices = new unsigned int[3];
		face.mNumIndices = 3;

		face.mIndices[0] = i * 3 + 0;
		face.mIndices[1] = i * 3 + 1;
		face.mIndices[2] = i * 3 + 2;
	}

	const std::string extension = filename.substr(filename.find_last_of(".") + 1);

	exporter.Export(&scene, extension, filename);
}

void Model::render(Shader& shader, Camera& camera, bool wireframe)
{
	// shader parameters
	shader.setUniform("objectColour", m_colour);
	shader.setUniform("lightColour", 1.0f, 1.0f, 1.0f);
	shader.setUniform("lightPos", 0.5f, 1.1f, 0.8f);
	shader.setUniform("viewPos", camera.getPosition());
	shader.setUniform("fade", 0.0f);
	shader.setUniform("wireframe", 1.0f);

	// Pass the matrices to the shader
	shader.setUniform("modelViewMatrix", getTransform());
	shader.setUniform("projectionMatrix", camera.getProjection());

	shader.bind();

	// Draw mesh
	m_mesh->draw(false);

	if (wireframe)
	{
		shader.setUniform("wireframe", 0.0f);
		shader.bind();

		m_mesh->draw(true);
	}
}
