#pragma once
#include <GL\glew.h>
#include <string>
#include <vector>

class Texturing
{
public:

	Texturing();
	~Texturing();

	void InitializeTexture(const std::string& file);
	void UseTexture(unsigned int number); //uses up to 32 textures	
	
	GLuint LoadCubemap(std::vector<std::string> faces);
	GLint GetTexHandler(unsigned int texID) { return textureHandler[texID]; }


private:

	GLuint textureHandler[4];
	unsigned int amountOfTextures;
};

