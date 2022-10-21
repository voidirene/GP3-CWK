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
	void SetTransformParameters(glm::vec3 movement, glm::vec3 rotation, glm::vec3 scale);
	void UpdateCollider(glm::vec3 position, float radius);
	void DisplayMesh(Mesh* mesh);
	void SetActive(bool active);

	Transform GetTransform() { return transform; }
	glm::mat4 getMM() { return transform.GetModel(); }

	BoundingSphere boundingSphere;

private:

	Transform transform;

	bool active;
	float counter = 0.0f;
};
