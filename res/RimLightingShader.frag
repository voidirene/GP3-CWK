#version 400 //TODO: rename?

out vec4 FragColor;

uniform vec3 lightDir;
in vec3 normal;

void main() 
{
	float intensity;
	vec4 color = vec4(0,1,1,1); //cyan

	intensity = 1.0 - dot(normal, lightDir); //TODO: maybe normalize(normal) instead?

	if (intensity <= 1 && intensity >= 0.6)
		color = vec4(1.0,0.5,0.5,1.0); //red
		
	FragColor.rgb = vec3(smoothstep(0.5, 1.0, color));
}
