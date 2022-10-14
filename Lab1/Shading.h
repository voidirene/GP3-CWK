#pragma once
#include <GL\glew.h>
#include "Transform.h"
#include <string>

class Shading
{
public:

	Shading();
	~Shading();

	void InitializeShader(const std::string& file);
	void InitializeGeoShader(const std::string& file);
	void UseShader();
	void UpdateTransform(const Transform& transform, const Camera& camera);


	#pragma region uniform setters
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), value);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(program, name.c_str()), value);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}


private:

	GLuint CreateNewShader(const std::string& name, unsigned int type);
	std::string LoadNewShader(const std::string& fileName);
	void CheckForErrors(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	GLuint program; //holds the shader program

	static const unsigned int numberOfShaders = 3; //amount of shaders
	GLuint shaders[numberOfShaders]; //array of shaders

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];
};

