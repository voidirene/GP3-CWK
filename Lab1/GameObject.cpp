#include "GameObject.h"

GameObject::GameObject()
{
	active = true;
}

void GameObject::LoadModel(const std::string& file)
{
	mesh.LoadMesh(file);
}

void GameObject::DisplayMesh()
{
	//TODO: check this out
	//transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
	//transform.SetScale(glm::vec3(0.1, 0.1, 0.1));
	mesh.Display();
}

void GameObject::SetActive(bool set)
{
	active = set;
}

void GameObject::SetTransformParameters(glm::vec3 movement, glm::vec3 rotation, glm::vec3 scaleChange)
{
	transform.SetPosition(movement);
	transform.SetRotation(rotation);
	transform.SetScale(scaleChange);
	UpdateCollider(*transform.GetPosition(), 0.62f); //TODO: un-hardcode the radius
}

void GameObject::UpdateCollider(glm::vec3 position, float radius)
{
	boundingSphere.UpdateSphereData(position, radius);
}