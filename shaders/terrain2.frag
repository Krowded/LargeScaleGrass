#version 150

out vec4 outColor;
in vec2 texCoord;

uniform sampler2D backgroundTexture;
uniform sampler2D normalMap;
uniform vec3 lightVector;
uniform mat3 normalTransformationMatrix;

void main(void)
{
	vec3 map = texture(normalMap, texCoord).xyz*0.0078125 - 128*0.0078125 ;
/*
	const vec3 reduction = vec3( -128*0.0078125, -128*0.0078125, -128*0.0078125 );
	vec3 normal = normalize((map * 0.0078125) + reduction);
	normal = normalTransformationMatrix * normal;
*/
	vec3 normal = normalTransformationMatrix * normalize(map);

	float diffuse = abs(dot(normal, lightVector));//max(dot(normal, lightVector), 0);

	outColor = diffuse * texture(backgroundTexture, texCoord);
	//outColor = vec4(normal,1);
}
