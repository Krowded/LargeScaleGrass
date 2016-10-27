#version 330

in vec3 frag_normal;
in vec3 frag_viewVector;
in vec2 frag_texCoords;
flat in float frag_hue; //Possibly add a second one for even more variation
flat in float textureType;

out vec4 out_Color;

uniform sampler2D tex;
uniform vec3 light;
uniform vec3 up;

const float skylight = 0.8;
const float earthlight = 0.3;
const float diffuseConstant = 0;//0.25;
const float specularConstant = 1;//0.2;
const float offset = 0.25;
void main(void)
{
	vec3 normal = normalize(frag_normal);
	if(!gl_FrontFacing)
		normal = -normal;
		
	//Hemispherical ambient lighting
	//float ratio = max(dot(normal, up), 0);
	float ratio = max(normal.y, 0.0); //Assuming y is up

	//float ambient = skylight*ratio + earthlight*(1-ratio);
	float ambient = mix(earthlight, skylight, ratio); //Optimized

	//Diffuse lighting
	float diffuse = max(dot(normal, light), 0); //Can be optimized if we decide on a stationary light source (but already pretty cheap)

	// Specular lighting
	vec3 r = reflect(light, normal);
	vec3 v = normalize(frag_viewVector); // View direction
	float specular = dot(r, v);
	specular = pow(max(specular,0), 150.0);

	//Add up
	float shade = min(1, (ambient + diffuseConstant*diffuse) + specularConstant*specular);
	
	/*
	//Get texture
	vec4 texOut = texture(tex, frag_texCoords*0.25 + textureType);
 	vec4 shades = vec4(frag_hue, 1.0, frag_hue, 1.0) * shade;
 	out_Color = texOut * shades;
 	*/

 	out_Color = texture(tex, frag_texCoords*0.25 + textureType) * shade;
 	out_Color.rb *= frag_hue;
	//out_Color = shade * vec4(frag_hue*texOut.r, texOut.g, frag_hue*texOut.b, texOut.w);
}
