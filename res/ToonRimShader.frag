#version 400 //TODO: rename?

out vec4 FragColor;

uniform vec3 lightDir;
in vec3 normal;

//TEXTURE STUFF
varying vec2 texCoord0;
uniform sampler2D diffuse;

void main() 
{
	float intensity;
	vec4 color = vec4(0,1,1,1); //cyan;
	
	//TEXTURE BELOW
	FragColor = texture2D(diffuse, texCoord0);

	//TOON BELOW
	intensity = dot(lightDir, normal); //TODO: maybe normalize(normal) instead?

	if (intensity > 0.95)
		color = vec4(0.2,0.0,0.5,0.5);
	else if (intensity > 0.5)
		color = vec4(0.1,0.0,0.3,0.2);
	else if (intensity > 0.25)
		color = vec4(0.1,0.0,0.1,0.2);
	else
		color = vec4(0.0,0.0,0.0,0.1);

	FragColor += color;

	//RIM BELOW
	intensity = 1.0 - dot(normal, lightDir); //TODO: maybe normalize(normal) instead?

	if (intensity <= 1 && intensity >= 0.6)
		color = vec4(0.1,0.1,0.8,1.0);
		
	FragColor.rgb += vec3(smoothstep(0.5, 1.0, color));
}
