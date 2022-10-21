#pragma once
#include "Camera.h"

struct Transform
{
public:

	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
	}
	inline glm::mat4 GetModel() const //runs as compile time
	{
		glm::mat4 posMat = glm::translate(pos);
		glm::mat4 scaleMat = glm::scale(scale);
		glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;
		return posMat * rotMat * scaleMat;
	}
	inline glm::vec3* GetPosition() { return &pos; } //getters
	inline glm::vec3* GetRotation() { return &rot; }
	inline glm::vec3* GetScale() { return &scale; }
	inline void SetPosition(glm::vec3& pos) { this->pos = pos; } // setters
	inline void SetRotation(glm::vec3& rot) { this->rot = rot; }
	inline void SetScale(glm::vec3& scale) { this->scale = scale; }

protected:
private:

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};