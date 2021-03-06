#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 normal;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform vec3 lightVector;

void main(void)
{
	mat3 normalTransformationMatrix = mat3(mdlMatrix);
	texCoord = inTexCoord;
	normal = normalTransformationMatrix * inNormal;
	
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
