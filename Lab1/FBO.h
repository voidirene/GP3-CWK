#pragma once
#include "Shading.h"
#include <GL/glew.h>
#include <iostream>

class FBO
{
public:

	void GenerateFBO(int width, int height); //TODO: width and height should match the ones in screenDisplay
	void BindFBO();
	void UnbindFBO();
	void RenderFBOtoQuad();
	void GenerateQuad();

private:


	GLuint FBO; // Frame Buffer 
	GLuint RBO; // Render Buffer 
	GLuint CBO; // Colour Buffer 
	GLuint quadVAO;
	GLuint quadVBO;

	Shading FBOShader;
};

