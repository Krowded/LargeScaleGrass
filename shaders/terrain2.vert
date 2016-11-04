#version 150

in  vec3 inPosition;
in vec2 inTexCoord;

out vec2 texCoord;

// NY
uniform mat3 normalTransformationMatrix;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform vec3 lightVector;



void main(void)
{
	texCoord = inTexCoord;
	
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
