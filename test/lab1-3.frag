#version 150

in vec3 frag_normal;
in float frag_hue;
in vec3 cameraVector;

out vec4 out_Color;

uniform vec3 light;
uniform vec3 up;

const float skylight = 0.7;
const float earthlight = 0.2;
const float diffuseConstant = 0.3;
const float specularConstant = 0.2;
void main(void)
{
	vec3 normal = normalize(frag_normal);
	if(!gl_FrontFacing)
		normal = -normal;
		

	//Hemispherical ambient lighting
	float ratio = dot(normal, up);

	float ambient = skylight*ratio + earthlight*(1-ratio);

	//Diffuse lighting
	float diffuse = diffuseConstant*dot(normal, light);

	// Specular lighting
	vec3 r = reflect(light, normal);
	vec3 v = normalize(-cameraVector); // View direction
	float specular = dot(r, v);
	if (specular > 0.0)
		specular = pow(specular, 150.0);
	specular = max(specularConstant*specular, 0.0);

	float shade = min(1, ambient + diffuse + specular);
	out_Color = shade * vec4(frag_hue*0.5, 0.7, frag_hue*0.5, 0.0);
}
