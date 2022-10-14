#version 400
//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;
//Uniform variable
uniform float randColourX;
uniform float randColourY;
uniform float randColourZ;

//TEXTURE STUFF
in vec2 TexCoords;
uniform sampler2D diffuse;
 
void main()
{
	//TEXTURE BELOW
	fragcolor = texture2D(diffuse, TexCoords);

	//Setting each vector component to uniform variable then setting final colour
	vec4 color;
	color = vec4(randColourX,randColourY,randColourZ,1.0);
    
	fragcolor += color;
}