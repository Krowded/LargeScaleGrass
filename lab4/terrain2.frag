#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 normal;
uniform sampler2D tex;
uniform vec3 lightVector;

void main(void)
{
	vec3 n = normalize(normal);
	float diffuse = max(dot(n, lightVector), 0);

	outColor = diffuse * texture(tex, texCoord);
} 
