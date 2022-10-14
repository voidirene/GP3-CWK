#include "ScreenDisplay.h"

ScreenDisplay::ScreenDisplay() : glew(0)
{
	window = nullptr;
	displayWidth = 1600.0f;
	displayHeight = 900.0f;
}

ScreenDisplay::~ScreenDisplay()
{
	glfwDestroyWindow(window); //delete the window
	SDL_Quit();
}

void ScreenDisplay::InitializeDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initialize SDL

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //set up double buffer

	glfwInit(); //initialize glfw

	window = glfwCreateWindow(displayWidth, displayHeight, "GP Coursework Game Window", NULL, NULL); //create game window

	glfwMakeContextCurrent(window); //set up context
	glew = glewInit(); //initialize glew
	glEnable(GL_DEPTH_TEST); //enables z-buffering 
	glEnable(GL_CULL_FACE); //does not allow faces that are not pointing to the camera to be displayed

	CheckForErrors(); //check that everything has been initialized properly, else return an error message

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f); //set background color
}

void ScreenDisplay::ChangeBuffer()
{
	glfwSwapBuffers(window);
}

void ScreenDisplay::ClearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears color and depth buffer and sets colour to colour defined in glClearColor
}

void ScreenDisplay::CheckForErrors()
{
	if (window == nullptr)
	{
		std::cout << "Window is a null pointer. Press any key to exit the app.";
		int in;
		std::cin >> in;
		SDL_Quit();
	}
	if (glew != GLEW_OK)
	{
		std::cout << "Glew initialization failed. Press any key to exit the app.";
		int in;
		std::cin >> in;
		SDL_Quit();
	}
}

float ScreenDisplay::GetWidth()
{
	return displayWidth;
}

float ScreenDisplay::GetHeight()
{
	return displayHeight;
}