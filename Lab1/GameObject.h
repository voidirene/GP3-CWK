#pragma once
#include "Mesh.h"
#include "Transform.h"

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

	glm::vec3 position;
	float radius;
};

class GameObject
{
public:

	GameObject();
	void LoadModel(const std::string& file);
	void SetTransformParameters(glm::vec3 movement, glm::vec3 rotation, glm::vec3 scale);
	void UpdateCollider(glm::vec3 position, float radius);
	void DisplayMesh();
	void SetActive(bool active); //TODO: make some functionality for this?

	Transform GetTransform() { return transform; }
	glm::mat4 GetModelMatrix() { return transform.GetModel(); }

	BoundingSphere boundingSphere;

private:

	Transform transform;
	Mesh mesh;

	bool active;
	float counter = 0.0f;
};
