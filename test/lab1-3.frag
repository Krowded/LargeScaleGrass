#version 150

in vec3 frag_normal;
in float frag_hue;
in vec3 cameraVector;
in vec2 frag_texCoords;
flat in int textureType;
out vec4 out_Color;

uniform sampler2D tex;
uniform vec3 light;
uniform vec3 up;

const float skylight = 0.7;
const float earthlight = 0.2;
const float diffuseConstant = 0.4;
const float specularConstant = 0.2;
const float offset = 0.25;
void main(void)
{
	vec3 normal = normalize(frag_normal);
	if(!gl_FrontFacing)
		normal = -normal;
		
	//Hemispherical ambient lighting
	float ratio = max(dot(normal, up), 0);
	float ambient = skylight*ratio + earthlight*(1-ratio);

	//Diffuse lighting
	float diffuse = max(diffuseConstant*dot(normal, light), 0);

	// Specular lighting
	vec3 r = reflect(light, normal);
	vec3 v = normalize(-cameraVector); // View direction
	float specular = dot(r, v);
	if (specular > 0.0)
		specular = pow(specular, 150.0);
	specular = max(specularConstant*specular, 0.0);

	float shade = min(1, ambient + diffuse + specular);
	//vec2 adjustedTexCoords = vec2((frag_texCoords.s+textureType)*0.25, frag_texCoords.t);
	vec4 texOut = texture(tex, frag_texCoords);
 	
	out_Color = shade * vec4(frag_hue*texOut.r, texOut.g, frag_hue*texOut.b, 0.0);
}
