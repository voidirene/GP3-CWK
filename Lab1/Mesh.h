#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "obj_loader.h"
#include <string>
#include "Texturing.h"
#include "Shading.h"

struct Vertex
{
public:

	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal)
	{
		this->position = pos;
		this->texCoordinates = texCoord;
		this->normal = normal;
	}

	glm::vec3* GetPosition() { return &position; }
	glm::vec2* GetTexCoordinates() { return &texCoordinates; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 position;
	glm::vec2 texCoordinates;
	glm::vec3 normal;
};

class Mesh
{
public:

	Mesh();
	~Mesh();

	void Display();
	void LoadMesh(const std::string& file); //for loading a model

	unsigned int displayCount; //how many VAOs we want to display

private:

	void InitializeMesh(Vertex* vertices, unsigned int numberOfVertices, unsigned int* indices, unsigned int numberOfIndices);
	void InitializeModel(const IndexedModel& model); //for initializing the model

	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,

		NUM_BUFFERS
	};

	GLuint vao;
	GLuint vaBuffers[NUM_BUFFERS]; //creates an array of buffers
};

