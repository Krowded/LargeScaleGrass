#version 330

layout(location = 1) in mat4 modelToWorld;
layout(location = 5) in float in_Thickness;
layout(location = 6) in float in_Hue;
//layout(location = 7) in vec2 in_Position;
//layout(location = 9) in vec2 in_Normal;
in vec2 in_TexCoords;
in mat4 tileTransformation;

flat out float textureType;
flat out float frag_hue;
out vec2 frag_texCoords;
out vec3 frag_normal;
out vec3 frag_viewVector;

uniform vec2 vertices[160]; //Pre-made grass shapes
uniform vec2 normals[160]; //Pre-made normals
uniform mat4 worldToProjection;
uniform float time;

//uniform mat4 globalLength //Can be used to simulate grass growing as well as give external control. Good for demo
//uniform float globalThickness //Ditto
//uniform float windSize

void main(void)
{
	int index = int(mod(gl_InstanceID,5))*32 + gl_VertexID;
	vec2 in_Position = vertices[index];
	vec2 in_Normal = normals[index];


	//Thickness tapers off towards top to make a pointy blade of grass
	//float thickness = (16-(gl_VertexID*0.5))*(1/16)*in_Thickness; //What it does
	float thickness = in_Thickness-(gl_VertexID*0.03125)*in_Thickness; //Optimized

	//Half the vertices expand towards z, other half towards -z
	//gl_Position = vec4(in_Position.xy, (1 - 2*mod(gl_VertexID,2))*thickness, 1.0);
	gl_Position.xzw = vec3(in_Position.x, thickness - 2*thickness*mod(gl_VertexID,2), 1.0); //Optimized
	//Add some "random" movement to make it seem more alive. Could make this scale so there's more movement at the tip, but don't think it's noticable enough.
	gl_Position.y = in_Position.y + 0.05*sin(time*0.05 + gl_InstanceID*0.5);
	
	//ModelToWorld transformations
	gl_Position =  tileTransformation * modelToWorld * gl_Position;
	
	//Wind force acting on the grass as an invisible cylinder moving across it (in the x-direction, leaving z untouched)
	const float windCylinderRadius = 5;
	vec2 windCylinderPosition = vec2(500*tan(time*0.0005)-2, windCylinderRadius); //- inverse(world)[3][2]);

	//Waterfall
//	vec4 windCylinderPosition1 = vec4(5*tan(time*0.005)-2, windCylinderRadius, 0,1);
//	vec2 windCylinderPosition = vec2(tileTransformation * windCylinderPosition1);
	
//Wind cont;
	vec2 windVector = gl_Position.xy - windCylinderPosition;
	float windDistance = length(windVector);
	float force = 0;// max(0, windCylinderRadius - windDistance); //Commented out cause nothing's working atm
	vec2 forceVector = vec2(pow(force,2), pow(force,5));
	gl_Position.xy += windVector.xy * forceVector.xy;

	//WorldToProjection transformations
	gl_Position = worldToProjection * gl_Position;	

	//Pass along stuff to fragment shader
	textureType = mod(gl_InstanceID,2)*0.25;
	frag_viewVector = -vec3(tileTransformation * modelToWorld * vec4(in_Position, 0, 1.0)); // For specular
	frag_hue = in_Hue;
	frag_texCoords = in_TexCoords;
	frag_normal = mat3(modelToWorld)*vec3(in_Normal,0); //Assuming no awkward scaling
}


