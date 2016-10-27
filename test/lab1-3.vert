#version 150

in vec2 in_Position; //Z-part always 0
in vec2 in_Normal; //Same here
in float in_Thickness;
in float in_Hue;
in vec2 in_TexCoords;
in mat4 model;

flat out int textureType;
out vec2 frag_texCoords;
out vec3 frag_normal;
out float frag_hue;
out vec3 cameraVector;

uniform mat4 world;
uniform mat4 projection; //Combine with world?
uniform mat4 tileTransformation;
uniform float time;

//uniform float globalLength //Can be used to simulate grass growing as well as give external control. Good for demo
//uniform float globalThickness //Ditto

void main(void)
{
	textureType = int(mod(gl_InstanceID,4)); //Thinking this reduces work in fragment shader. Could be wrong?

	//Thickness tapers off towards top to make a pointy blade of grass
	float thickness = (16-(gl_VertexID*0.5))*0.0625*in_Thickness;

	//Half the vertices expand towards z, other half towards -z (written like this to avoid branching)
	gl_Position = vec4(in_Position.x, in_Position.y, (1 - 2*mod(gl_VertexID,2))*thickness, 1.0);
	gl_Position = tileTransformation * model * gl_Position;
	
	//Wind force acting on the grass as an invisible cylinder moving across it
	float windCylinderRadius = 1.999;
	vec2 windCylinderPosition = vec2(5*sin(time*0.005)-3, 2);
	vec2 windVector = vec2(gl_Position.x, gl_Position.y) - windCylinderPosition;
	float windDistance = length(windVector);
	float force = max(0, windCylinderRadius - windDistance);
	gl_Position = vec4( gl_Position.x + windVector.x*pow(force,2), gl_Position.y + windVector.y*pow(force,3), gl_Position.z, 1.0);

	gl_Position = projection * world * gl_Position;	

	cameraVector = vec3(world * model * vec4(in_Position, 0, 1.0)); // 
	frag_hue = in_Hue;
	frag_texCoords = in_TexCoords;
	frag_normal = mat3(projection*world*model)*vec3(in_Normal,0);
}


