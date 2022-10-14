#include "Shading.h"
#include <fstream>
#include <iostream>

Shading::Shading() : program(0)
{
	memset(shaders, 0, sizeof(shaders));
	memset(uniforms, 0, sizeof(uniforms));
}

Shading::~Shading()
{
	for (int i = 0; i < numberOfShaders; i++) 
	{
		glDetachShader(program, shaders[i]); //detach shaders from program
		glDeleteShader(shaders[i]); //delete the shaders
	}
	glDeleteProgram(program); //delete the program
}

void Shading::InitializeShader(const std::string& file)
{
	program = glCreateProgram(); //create the program

	shaders[0] = CreateNewShader(LoadNewShader(file + ".vert"), GL_VERTEX_SHADER); //create the shaders from file
	shaders[1] = CreateNewShader(LoadNewShader(file + ".frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < numberOfShaders; i++) //for loop for attaching shaders to the program
	{
		glAttachShader(program, shaders[i]);
	}

	glBindAttribLocation(program, 0, "position"); //bind the attribute locations
	glBindAttribLocation(program, 1, "texCoord");

	glLinkProgram(program); //create executables that will run on the GPU shaders
	CheckForErrors(program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); //check if it has linked

	glValidateProgram(program); //check the entire program is valid
	CheckForErrors(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform"); //sets up the uniform with the shader program
}

void Shading::InitializeGeoShader(const std::string& file)
{
	program = glCreateProgram(); //create the program

	shaders[0] = CreateNewShader(LoadNewShader(file + ".vert"), GL_VERTEX_SHADER); //create the shaders from file
	shaders[1] = CreateNewShader(LoadNewShader(file + ".frag"), GL_FRAGMENT_SHADER);
	shaders[2] = CreateNewShader(LoadNewShader(file + ".geom"), GL_GEOMETRY_SHADER);

	for (unsigned int i = 0; i < numberOfShaders; i++) //for loop for attaching shaders to the program
	{
		glAttachShader(program, shaders[i]);
	}

	glBindAttribLocation(program, 0, "position"); //bind the attribute locations
	glBindAttribLocation(program, 1, "texCoord");

	glLinkProgram(program); //create executables that will run on the GPU shaders
	CheckForErrors(program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); //check if it has linked

	glValidateProgram(program); //check the entire program is valid
	CheckForErrors(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform"); //sets up the uniform with the shader program
}

void Shading::UseShader()
{
	glUseProgram(program); //use the shader program
}

GLuint Shading::CreateNewShader(const std::string& file, unsigned int shaderType)
{
	GLuint shader = glCreateShader(shaderType); //create shader based on the specified shaderType

	if (shader == 0)
		std::cerr << "Failed to create shader of type " << shaderType << '\n';

	const GLchar* source[1];
	source[0] = file.c_str(); //converts the string into a char array
	GLint length[1];
	length[0] = file.length();

	glShaderSource(shader, 1, source, length); //sends the source code to openGL
	glCompileShader(shader); //gets openGL to compile the shader code

	CheckForErrors(shader, GL_COMPILE_STATUS, false, "Shader compilation failed"); //checks for compilation error

	return shader;
}

std::string Shading::LoadNewShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());
	std::string output;
	std::string line;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + '\n');
		}
	}
	else
	{
		std::cerr << "Failed to load shader: " << fileName << '\n';
	}
	return output;
}

void Shading::UpdateTransform(const Transform& transform, const Camera& camera)
{
	glm::mat4 mvp = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}

void Shading::CheckForErrors(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		std::cerr << errorMessage << ": '" << error << "'" << '\n';
	}
}