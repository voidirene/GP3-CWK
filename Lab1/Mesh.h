#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "obj_loader.h"
#include <string>
#include "Transform.h"
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

struct BoundingSphere //for collision detection
{
public:

	BoundingSphere()
	{

	}

	void UpdateSphereData(glm::vec3 position, float radius)
	{
		this->position = position;
		this->radius = radius;
	}

	glm::vec3 GetPosition() { return position; }
	float GetRadius() { return radius; }

private:

	float radius;
	glm::vec3 position;
};

class Mesh
{
public:

	Mesh();
	~Mesh();

	void Display(float movementX, float movementY, float movementZ, float rotationX, float rotationY, float rotationZ, float scaleChange, Camera camera);
	void LoadModel(const std::string& file); //for loading a model

	void UpdateTransformValues(float movementX, float movementY, float movementZ, float rotationX, float rotationY, float rotationZ, float scaleChange);
	void UpdateSphereData(glm::vec3 position, float radius);

	unsigned int displayCount; //how many VAOs we want to display

	Transform transform;
	BoundingSphere boundingSphere;

	glm::mat4 getMM() { return transform.GetModel(); }

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

