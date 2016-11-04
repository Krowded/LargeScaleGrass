// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <math.h>
#include "Skybox.h"
#include "GrassHandler.h"
#include <iostream>

bool isGravityOn = true;
const GLfloat standardDistanceFromGround = 6;


GLfloat windowWidth;
GLfloat windowHeight;
GLint drawDistance;
GLfloat nearDrawDistance;
GLfloat fov;

// Reference to shader program
GLuint modelProgram, terrainProgram;

//Texture references
GLuint terrainTexture;

mat4 camMatrix, camBaseMatrix, projectionMatrix;

GLfloat cameraHeight;

GLuint numberOfTiles;
vec3 *tileNormalsArray;

float TerrainHeight(vec3);

Model* GenerateTerrain(TextureData *tex)
{
	GLuint vertexCount = tex->width * tex->height;
	GLuint triangleCount = (tex->width-1) * (tex->height-1) * 2;
	GLuint x, z;
	
	GLfloat *vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = (GLfloat*)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount*3);
	
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (GLfloat)x/10.0f;//(float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = (GLfloat)z/10.0f;//(float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		// Normal vectors. You need to calculate these.
			if (x > 0 && x < tex->width-1 && z > 0 && z < tex->height-1 )
			{
				vec3 leftRightVector = SetVector( (vertexArray[(x - 1 + z * tex->width)*3 + 0] - vertexArray[(x + 1 + z * tex->width)*3 + 0]),
												  (vertexArray[(x - 1 + z * tex->width)*3 + 1] - vertexArray[(x + 1 + z * tex->width)*3 + 1]),
												  (vertexArray[(x - 1 + z * tex->width)*3 + 2] - vertexArray[(x + 1 + z * tex->width)*3 + 2]));

				vec3 upDownVector = SetVector( (vertexArray[(x + (z - 1) * tex->width)*3 + 0] - vertexArray[(x + (z + 1) * tex->width)*3 + 0]),
											   (vertexArray[(x + (z - 1) * tex->width)*3 + 1] - vertexArray[(x + (z + 1) * tex->width)*3 + 1]),
											   (vertexArray[(x + (z - 1) * tex->width)*3 + 2] - vertexArray[(x + (z + 1) * tex->width)*3 + 2]));
				vec3 normalVector = Normalize(CrossProduct(upDownVector, leftRightVector));
				normalArray[(x + z * tex->width)*3 + 0] = normalVector.x;
				normalArray[(x + z * tex->width)*3 + 1] = normalVector.y;
				normalArray[(x + z * tex->width)*3 + 2] = normalVector.z;
			}
			else
			{
				normalArray[(x + z * tex->width)*3 + 0] = 0.0;
				normalArray[(x + z * tex->width)*3 + 1] = 1.0;
				normalArray[(x + z * tex->width)*3 + 2] = 0.0;
			}
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *tm;
// Reference to shader program
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	dumpInfo();
	
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	vec3 cam = {0, 0, 0};
	vec3 lookAtPoint = {0, 0, -4};
	vec3 upVector = {0.0, 1.0, 0.0};
	mat4 projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
	camBaseMatrix = lookAt(cam.x, cam.y, cam.z,
			    lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				upVector.x, upVector.y, upVector.z);
	camMatrix = camBaseMatrix;

	// Load and compile shader
	terrainProgram = loadShaders((char*)"shaders/terrain.vert", (char*)"shaders/terrain.frag");
	modelProgram = loadShaders((char*)"shaders/models.vert", (char*)"shaders/models.frag");

	glUseProgram(terrainProgram);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(terrainProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(terrainProgram, "backgroundTexture"), 0); // Texture unit 0
	LoadTGATextureSimple((char*)"textures/grass_texture237.tga", &tex1);

	glUseProgram(modelProgram);
	glUniformMatrix4fv(glGetUniformLocation(modelProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Load terrain data
	LoadTGATextureData((char*)"textures/fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	InitSkybox(projectionMatrix);
	GLuint numberOfTiles = (ttex.width)*(ttex.height);
	InitGrass(projectionMatrix, numberOfTiles, (vec3*)(tm->vertexArray), (vec3*)(tm->normalArray));
	free(tileNormalsArray); //Not using it anymore


	// Load models
	m = LoadModelPlus((char*)"models/groundsphere.obj");
}


vec3 lightVector = {1, 1, 0};
void display(void)
{
	static GLint t = 0;
	++t;
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	printError("pre display");

	mat4 total, modelView;	

	vec3 rotatedLightVector = MultMat3Vec3(mat4tomat3(camMatrix),lightVector);

	//skybox
	glDisable(GL_DEPTH_TEST);
	DrawSkybox(camMatrix);
	glEnable(GL_DEPTH_TEST);

	printError("display skybox");

	//Draw grass
	DrawGrass(t, camMatrix, lightVector);

	printError("display grass");

	//Draw terrain
	glUseProgram(terrainProgram);
	glUniformMatrix4fv(glGetUniformLocation(terrainProgram, "mdlMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniform3fv(glGetUniformLocation(terrainProgram, "lightVector"), 1, &rotatedLightVector.x);	
	glBindTexture(GL_TEXTURE_2D, tex1);
	DrawModel(tm, terrainProgram, (char*)"inPosition", (char*)"inNormal", (char*)"inTexCoord");

	printError("display terrain");


	//Draw models
	glUseProgram(modelProgram);

	modelView =T(25*t*0.001, 0, 25*t*0.001);
	
	static float lastHeight = 0;
	vec3 currentPosition = SetVector((modelView.m)[3], (modelView.m)[7], (modelView.m)[11]); 

	float height = TerrainHeight(currentPosition);
	float heightDifference = height - lastHeight;
	(modelView.m)[7] = lastHeight + heightDifference;
	lastHeight = (modelView.m)[7];

	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(m, modelProgram, (char*)"inPosition", (char*)"inNormal", (char*)"inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void CameraControl(int);

void timer(int time)
{
	CameraControl(time);
	glutPostRedisplay();

	static GLfloat lastTick = 0;
	static GLint fps = 0;
	fps++;
	if(time - lastTick > 1000) {
		printf("FPS: %d\n", fps);
		lastTick = time;
		fps = 0;
	}


	glutTimerFunc(10, &timer, glutGet(GLUT_ELAPSED_TIME));
}

mat4 directions;
vec3 backDirectionVec, rightDirectionVec, upDirectionVec;

void CameraMouseUpdate(int mouseX, int mouseY)
{	
		static GLfloat x = 0;
		static GLfloat y = 0;	

		//Update mousepointer value
		if (mouseY != 0)
		{
			x = (GLfloat)mouseX;

			//Clamp Y
			if (mouseY < 375 && mouseY > 276)
			y = (GLfloat)mouseY;
		}

		camMatrix = Mult(Ry(2*M_PI*x/512), Mult( ArbRotate(rightDirectionVec, 2*M_PI*y/324), camBaseMatrix));		
}


float TerrainHeight(vec3 currentPosition)
{
	int x,z;
	//Find the quad
	currentPosition.x = currentPosition.x;
	currentPosition.z = currentPosition.z;
	float x1 = floor(currentPosition.x);
	float x2 = floor(currentPosition.x + 1);

	float z1 = floor(currentPosition.z);
	float z2 = floor(currentPosition.z + 1);

	//Determine the triangle
	if( x1 < 0 || z1 < 0 || x2 > ttex.width || z2 > ttex.height ) //Outside texture
	{
		return -100;
	}
	else if( (x2 - currentPosition.x) > (currentPosition.z - z1)) //Left triangle
	{
		x = (int)x1;
		z = (int)z1;

	}
	else //Right triangle
	{
		x = (int)x2;
		z = (int)z2;
	}

	vec3 startPoint = SetVector(tm->vertexArray[(x + z * ttex.width)*3 + 0], 
		                        tm->vertexArray[(x + z * ttex.width)*3 + 1], 
		                        tm->vertexArray[(x + z * ttex.width)*3 + 2]);

	vec3 vectorX = SetVector( tm->vertexArray[((int)x2 + (int)z1 * ttex.width)*3 + 0],
							  tm->vertexArray[((int)x2 + (int)z1 * ttex.width)*3 + 1],
							  tm->vertexArray[((int)x2 + (int)z1 * ttex.width)*3 + 2]);
	vectorX = VectorSub(vectorX, startPoint);

	vec3 vectorZ = SetVector( tm->vertexArray[((int)x1 + (int)z2 * ttex.width)*3 + 0],
							  tm->vertexArray[((int)x1 + (int)z2 * ttex.width)*3 + 1],
							  tm->vertexArray[((int)x1 + (int)z2 * ttex.width)*3 + 2]);
	vectorZ = VectorSub(vectorZ, startPoint);

	vec3 normal = Normalize(CrossProduct(vectorZ, vectorX));

	return (DotProduct(normal, startPoint) - normal.x * currentPosition.x - normal.z * currentPosition.z) / normal.y;
}

int tlast = 0;
void CameraControl(int time)
{
	int passedTime = time - tlast;
	tlast = time;	
	GLfloat averageSpeed = 0.02;
	GLfloat speed = (GLfloat)passedTime * averageSpeed;

	directions = Transpose(camMatrix); //Taking the inverse, kinda pointless
	backDirectionVec = Normalize(SetVector( (directions.m)[2], (directions.m)[6], (directions.m)[10]));
	rightDirectionVec = Normalize(SetVector( (directions.m)[0], (directions.m)[4], (directions.m)[8]));	
	upDirectionVec = Normalize(SetVector( (directions.m)[1], (directions.m)[5], (directions.m)[9]));

	//Cant move in y direction, y direction removed, speed added
	vec3 backVec = ScalarMult( Normalize(SetVector(backDirectionVec.x, 0, backDirectionVec.z)), speed);
	vec3 rightVec = ScalarMult( Normalize(SetVector(rightDirectionVec.x, 0, rightDirectionVec.z)), speed);
	//vec3 upVec = ScalarMult( Normalize(SetVector(upDirectionVec.x, 0, upDirectionVec.z)), speed);
	if (glutKeyIsDown('w'))
		camBaseMatrix = Mult( T( backVec.x, backVec.y, backVec.z), camBaseMatrix);
	if (glutKeyIsDown('a')) 
		camBaseMatrix = Mult( T( rightVec.x, rightVec.y, rightVec.z), camBaseMatrix);
	if (glutKeyIsDown('s') )
		camBaseMatrix = Mult( T( -backVec.x, -backVec.y, -backVec.z), camBaseMatrix);
	if (glutKeyIsDown('d'))
		camBaseMatrix = Mult( T( -rightVec.x, -rightVec.y, -rightVec.z), camBaseMatrix);
	if (glutKeyIsDown('q'))
		camBaseMatrix = Mult( T( 0, 1, 0), camBaseMatrix);
	if (glutKeyIsDown('e'))
		camBaseMatrix = Mult( T( 0, -1, 0), camBaseMatrix);

	if(glutKeyIsDown(' '))
		isGravityOn = !isGravityOn;

	//Adjust height to map
	if(isGravityOn)
	{
		vec3 currentPosition = SetVector(-(camBaseMatrix.m)[3], -(camBaseMatrix.m)[7], -(camBaseMatrix.m)[11]);
		float height = -TerrainHeight(currentPosition) - standardDistanceFromGround;
		if (height < 95)
		{
			float heightDifference = (camBaseMatrix.m)[7] - height;
			(camBaseMatrix.m)[7] = (camBaseMatrix.m)[7] - heightDifference;
		}
	}

	//Pass it off to update camMatrix
	CameraMouseUpdate(0,0);
}

void Destructor() {}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1024, 576);
	glutCreateWindow ((char*)"Field of grass");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, glutGet(GLUT_ELAPSED_TIME));

	glutPassiveMotionFunc(CameraMouseUpdate);

	glutMainLoop();

	atexit(GrassDestructor);
	exit(0);
}
