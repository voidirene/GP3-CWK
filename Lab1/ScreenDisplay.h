#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <glfw3.h>
#include <string>
#include <iostream>
using std::cout;

class ScreenDisplay
{
public:

	ScreenDisplay();
	~ScreenDisplay();

	void InitializeDisplay(); //This function initializes the systems required for a new window screen
	void ChangeBuffer(); //This function swaps buffers (since we're using double buffering)
	void ClearDisplay(float r, float g, float b, float a);

	float GetWidth();
	float GetHeight();

	GLFWwindow* window; //Variable that holds a pointer to our game window

private:

	void CheckForErrors(); //This function checks that all systems initialized in initializeDisplay() have initialized correctly

	GLenum glew;

	float displayWidth; //the game window's width
	float displayHeight; //the game window's height
};

