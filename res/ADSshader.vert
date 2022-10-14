#version 330 core //TODO: rename

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

// texture
layout (location = 1) in vec2 texCoord;

out VS_OUT {
    vec2 texCoord0;
} gs_out;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);

	// texture
	gs_out.texCoord0 = texCoord;
}