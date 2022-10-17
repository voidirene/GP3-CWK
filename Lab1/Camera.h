#pragma once
#include "ScreenDisplay.h"
#include <glfw3.h>
#include <glm/gtx/vector_angle.hpp>

struct Camera
{
public:

	Camera()
	{

	}

	void InitializeCamera(const glm::vec3& position, float fov, float aspectRatio, float nearClip, float farClip)
	{
		this->cameraPosition = position;
		this->forwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
		this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projectionMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
	}

	glm::vec3 GetPosition()
	{
		return this->cameraPosition;
	}

	inline glm::mat4 GetViewProjection() const //creates projection matrix
	{
		return projectionMatrix * glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
	}

	inline glm::mat4 GetProjection() const
	{
		return projectionMatrix;
	}

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
	}

	void MouseControls(ScreenDisplay* display)
	{
		if (glfwGetMouseButton(display->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) //if left mouse buttons is pressed, do camera rotation stuff
		{
			glfwSetInputMode(display->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //hide cursor

			//get the values for the screen's width and height, so the getters don't have to be called all the time
			float screenWidth = display->GetWidth();
			float screenHeight = display->GetHeight();

			//this stops the camera from doing a small 'leap' when the left mouse button is pressed
			if (firstPress)
			{
				glfwSetCursorPos(display->window, (screenWidth / 2), (screenHeight / 2));
				firstPress = false;
			}

			double x, y;
			glfwGetCursorPos(display->window, &x, &y); //get the cursor's coordinates

			//moves the coordinates of the cursor such that they begin in the middle of the screen and then changes them into degrees
			//the 90 is a value for sensitivity, a higher number means a faster camera
			float xRotation = 90 * (float)(y - (screenHeight / 2)) / screenHeight;
			float yRotation = 90 * (float)(x - (screenWidth / 2)) / screenWidth;

			//rotate left and right
			forwardVector = glm::rotate(forwardVector, glm::radians(-yRotation), upVector);

			//calculate the vertical change in the forward vector
			glm::vec3 forwardNew = glm::rotate(forwardVector, glm::radians(-xRotation), glm::normalize(glm::cross(forwardVector, upVector)));

			//check if the new forward vector is legal
			if (abs(glm::angle(forwardNew, upVector) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				forwardVector = forwardNew;
			}

			glfwSetCursorPos(display->window, (screenWidth / 2), (screenHeight / 2)); //set cursor in the middle of the screen
		}
		else if (glfwGetMouseButton(display->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) //if left mouse button is released, leave 'rotation mode'
		{
			glfwSetInputMode(display->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //unhide cursor
			firstPress = true; //set this to true again
		}
	}

	//the 0.02 is a value for sensitivity, a higher number means a faster camera
	void MoveCameraHorizontally(float direction)
	{
		cameraPosition.x += direction;
	}

	void MoveCameraVertically(float direction)
	{
		cameraPosition.y += direction;
	}

	void ZoomCamera(float direction)
	{
		cameraPosition.z += direction;
	}

	void CenterCameraOnMesh(glm::vec3 meshPosition, float distance)
	{
		forwardVector = glm::vec3(glm::normalize(meshPosition - cameraPosition));

		//center camera on mesh position
		cameraPosition.x = meshPosition.x;
		cameraPosition.y = meshPosition.y;

		//move the camera back a bit and reset the forward vector
		cameraPosition.z = distance;
		forwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	void PointCameraAtMesh(glm::vec3 meshPosition)
	{
		forwardVector = glm::vec3(glm::normalize(meshPosition - cameraPosition));
	}

	void RotateCameraAroundMesh(glm::vec3 meshPosition, float direction)
	{
		forwardVector = glm::vec3(glm::normalize(meshPosition - cameraPosition));
		cameraPosition += glm::cross(upVector, forwardVector) * direction;
	}

	bool firstPress = true;

private:

	glm::vec3 cameraPosition;
	glm::mat4 projectionMatrix;
	glm::vec3 forwardVector;
	glm::vec3 upVector;
};
