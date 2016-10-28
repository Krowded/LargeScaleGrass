#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <cmath>
#include <iostream>
#include "GrassTypes.h"
#include "LoadTGA.h"

#define tileSize 1.0f

struct Tile {
	vec3 position;
	mat4 transformation;
	GLuint offset;
};

GLfloat tilePositions[] = {
	-tileSize, 0.0f, -tileSize,
	0.0f, 0.0f, -tileSize,
	tileSize, 0.0f, -tileSize,
	-tileSize, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	tileSize, 0.0f, 0.0f,
	-tileSize, 0.0f, tileSize,
	0.0f, 0.0f, tileSize,
	tileSize, 0.0f, tileSize,
	-2*tileSize, 0.0f, -2*tileSize,
	0.0f, 0.0f, -2*tileSize,
	2*tileSize, 0.0f, -2*tileSize,
	-2*tileSize, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	2*tileSize, 0.0f, 0.0f,
	-2*tileSize, 0.0f, 2*tileSize,
	0.0f, 0.0f, 2*tileSize,
	2*tileSize, 0.0f, 2*tileSize,
};

const GLfloat baseSizeModifier = 1;
const GLint maxGrassPerTile = 1000;
const GLint numberOfStraws = 16000;
const GLfloat grassMinSize = 0.002;
const GLfloat grassMaxSize = 0.05;
const GLfloat grassMinThickness = 0.01;
const GLfloat grassMinHue = 0.1;

GLuint grassShader;
GLuint grassTexture;
GLfloat globalTime = 0;

//One VAO for all the grass
GLuint grassVAO;
GLuint tileVBO;
GLuint tileTransformationLocation;

//Tiles
GLuint numberOfTiles = 18;
mat4 tileTransformations[1000];
Tile* Tiles;


mat4 grassTransformations[numberOfStraws];
GLfloat thickness[numberOfStraws];
GLfloat hue[numberOfStraws];
GLuint textureType[numberOfStraws];


void generateGrass() {
	//Procedural generation of the grass
	srand(0);
	for(int i = 0; i < numberOfStraws; ++i) {
		GLfloat length = (float(rand() % 1000*grassMaxSize)/1000.0f + grassMinSize) * baseSizeModifier;
		GLfloat angle = M_PI * float(rand() % 1000)/500.0f;
		GLfloat displacementx = tileSize * float(rand() % 1000)/1000.0f;
		GLfloat displacementz = tileSize * float(rand() % 1000)/1000.0f;

		mat4 scaling = S(length, length, length);
		mat4 rotation = Ry(angle);
		mat4 translation = T(displacementx, 0, displacementz);	
		grassTransformations[i] = Transpose(Mult(translation, Mult(rotation, scaling)));


		thickness[i] = (rand() % 1500)/10000.0f + grassMinThickness;
		hue[i] = float((rand() % 1500))/1000.0f + grassMinHue;
 	}

	//Generate VAOs, VBOs
	GLuint indexBufferObjID, texCoordBufferObjID, grassTransformationBuffer, 
		   grassHueBuffer, grassThicknessBuffer, grassVertexBuffer, grassNormalBuffer;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVertexBuffer);
	glGenBuffers(1, &grassNormalBuffer);
	glGenBuffers(1, &indexBufferObjID);
	glGenBuffers(1, &texCoordBufferObjID);
    glGenBuffers(1, &grassTransformationBuffer);
    glGenBuffers(1, &grassHueBuffer);
    glGenBuffers(1, &grassThicknessBuffer);

    GLuint VAO = grassVAO;
    glBindVertexArray(VAO);
	
	//Upload vertex attribute arrays
	glBindBuffer(GL_ARRAY_BUFFER, grassTransformationBuffer);
	glBufferData(GL_ARRAY_BUFFER, numberOfStraws * sizeof(mat4), grassTransformations, GL_STATIC_DRAW);
    
    // Set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(glGetAttribLocation(grassShader, "modelToWorld")); 
    glVertexAttribPointer(glGetAttribLocation(grassShader, "modelToWorld"), 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)0);
    glEnableVertexAttribArray(glGetAttribLocation(grassShader, "modelToWorld")+1); 
    glVertexAttribPointer(glGetAttribLocation(grassShader, "modelToWorld")+1, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(sizeof(vec4)));
    glEnableVertexAttribArray(glGetAttribLocation(grassShader, "modelToWorld")+2); 
    glVertexAttribPointer(glGetAttribLocation(grassShader, "modelToWorld")+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(2 * sizeof(vec4)));
    glEnableVertexAttribArray(glGetAttribLocation(grassShader, "modelToWorld")+3); 
    glVertexAttribPointer(glGetAttribLocation(grassShader, "modelToWorld")+3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(3 * sizeof(vec4)));

    glVertexAttribDivisor(glGetAttribLocation(grassShader, "modelToWorld"), 1);
    glVertexAttribDivisor(glGetAttribLocation(grassShader, "modelToWorld")+1, 1);
    glVertexAttribDivisor(glGetAttribLocation(grassShader, "modelToWorld")+2, 1);
    glVertexAttribDivisor(glGetAttribLocation(grassShader, "modelToWorld")+3, 1);

    printError("upload model");

    glBindBuffer(GL_ARRAY_BUFFER, grassHueBuffer);
	glBufferData(GL_ARRAY_BUFFER, numberOfStraws * sizeof(GLfloat), hue, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_Hue")); 
    glVertexAttribPointer(glGetAttribLocation(grassShader, "in_Hue"), 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
    glVertexAttribDivisor(glGetAttribLocation(grassShader, "in_Hue"), 1);

    printError("upload hues");

	glBindBuffer(GL_ARRAY_BUFFER, grassThicknessBuffer);
	glBufferData(GL_ARRAY_BUFFER, numberOfStraws * sizeof(GLfloat), thickness, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_Thickness")); 
    glVertexAttribPointer(glGetAttribLocation(grassShader, "in_Thickness"), 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
    glVertexAttribDivisor(glGetAttribLocation(grassShader, "in_Thickness"), 1);

    printError("upload thickness");

	//Upload indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);

	printError("upload indices");

	//Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, vertNum*2*sizeof(GLfloat), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(grassShader, "in_TexCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_TexCoords"));

	printError("upload texture coordinates");

	glUniform2fv(glGetUniformLocation(grassShader, "vertices"), 160, combinedVertices);

	printError("upload position");

	glUniform2fv(glGetUniformLocation(grassShader, "normals"), 160, combinedNormals);

	printError("upload normal");

    glBindVertexArray(0);
}	

void generateTiles() {
	Tiles = (Tile*)malloc(numberOfTiles*sizeof(Tile));
	for(int i = 0; i < numberOfTiles; ++i)
	{
		Tiles[i].position = ((vec3*)tilePositions)[i];
		Tiles[i].offset = rand() % (numberOfStraws - maxGrassPerTile*5);
	}


	glGenBuffers(1, &tileVBO);
	tileTransformationLocation = glGetAttribLocation(grassShader, "tileTransformation");
    printError("generate tiles");

}


void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.5,0.5,0.5,0);
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	grassShader = loadShaders("lab1-3.vert", "lab1-3.frag");
	printError("init shader");
	
	//Upload textures
	glUniform1i(glGetUniformLocation(grassShader, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple((char*)"texture.tga", &grassTexture);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	printError("upload textures");

	// Generate and upload geometry to the GPU:
	generateGrass();
	generateTiles();

	// End of upload of geometry
	
	printError("init arrays");
}



GLfloat xview = 0.0f;
GLfloat yview = 0.0f;
GLfloat xangle = 0.0f;
void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	printError("pre display");

	mat4 worldMatrix = 
	{
		cos(globalTime/1000.0f), 0.0f, -sin(globalTime/1000.0f), -xview,
		0.0f,                1.0f, 0.0f,                 -yview,
		sin(globalTime/1000.0f), 0.0f, cos(globalTime/1000.0f),  0.0f,
		0.0f,                0.0f, 0.0f,                 1.0f 
	};

	//worldMatrix = Ry(M_PI/2);
	//worldMatrix = Mult(T(-xview,-yview,0),worldMatrix);
	worldMatrix = T(-xview,-yview,0);

	worldMatrix = Mult(Rx(xangle), worldMatrix);

	mat4 projectionMatrix = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};
	
	//Upload matrices
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "worldToProjection"), 1, GL_TRUE, Mult(projectionMatrix, worldMatrix).m);

	//Light vectors
	vec3 upVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(0,1, -1));
	glUniform3fv(glGetUniformLocation(grassShader, "up"), 1, &upVector.x);
	vec3 lightVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(1,1,0));
	glUniform3fv(glGetUniformLocation(grassShader, "light"), 1, &lightVector.x);

	//Time
	glUniform1f(glGetUniformLocation(grassShader, "time"), globalTime);

	//Draw the grass
	glDisable(GL_CULL_FACE); //Both sides need to be visible
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
	//Set tileTransformation vertex attribute array
	GLuint totalLevelsOfDetail = 1;
	GLuint levelOfDetail[] = { 5000 };
	GLuint strawCount = 0;
	for(int i = 0; i < totalLevelsOfDetail; ++i) {

		for(int j = 0; j < numberOfTiles; ++j) {
			tileTransformations[j] = IdentityMatrix();//T(Tiles[j].position.x, Tiles[j].position.y, Tiles[j].position.z);
		}
		glBufferData(GL_ARRAY_BUFFER, numberOfTiles * sizeof(mat4), tileTransformations, GL_STATIC_DRAW);
		// Set attribute pointers for matrix (4 times vec4)
	    glEnableVertexAttribArray(tileTransformationLocation); 
	    glVertexAttribPointer(tileTransformationLocation, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)0);
	    glEnableVertexAttribArray(tileTransformationLocation+1); 
	    glVertexAttribPointer(tileTransformationLocation+1, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(sizeof(vec4)));
	    glEnableVertexAttribArray(tileTransformationLocation+2); 
	    glVertexAttribPointer(tileTransformationLocation+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(2 * sizeof(vec4)));
	    glEnableVertexAttribArray(tileTransformationLocation+3); 
	    glVertexAttribPointer(tileTransformationLocation+3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(3 * sizeof(vec4)));

		GLuint strawsPerTile = levelOfDetail[i];

		std::cout << std::endl;
		std::cout << (tileTransformations[0].m)[0] << " " << (tileTransformations[0].m)[1] << " " << (tileTransformations[0].m)[2] << " " << (tileTransformations[0].m)[3] <<  std::endl;
		std::cout << (tileTransformations[0].m)[4] << " " << (tileTransformations[0].m)[5] << " " << (tileTransformations[0].m)[6] << " " << (tileTransformations[0].m)[7] <<  std::endl;
		std::cout << (tileTransformations[0].m)[8] << " " << (tileTransformations[0].m)[9] << " " << (tileTransformations[0].m)[10] << " " << (tileTransformations[0].m)[11] <<  std::endl;
		std::cout << (tileTransformations[0].m)[12] << " " << (tileTransformations[0].m)[13] << " " << (tileTransformations[0].m)[14] << " " << (tileTransformations[0].m)[15] <<  std::endl;
		glVertexAttribDivisor(tileTransformationLocation, strawsPerTile);
		glVertexAttribDivisor(tileTransformationLocation+1, strawsPerTile);
	    glVertexAttribDivisor(tileTransformationLocation+2, strawsPerTile);
	    glVertexAttribDivisor(tileTransformationLocation+3, strawsPerTile);

		//Draw all the straws at this LOD
	    glDrawElementsInstancedBaseVertexBaseInstance( GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0L, numberOfTiles*strawsPerTile, 0, 10);
	    strawCount += numberOfTiles*strawsPerTile;
	}
	if(int(globalTime) % 1000 < 1)
		std::cout << strawCount << std::endl;

	printError("display");
	
	glutSwapBuffers();
}

void KeyboardInput(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {  		
    case 'w':
    	yview += 0.1f;
    	break;
    case 's':
    	yview -= 0.1f;
    	break;
    case 'a':
    	xview -= 0.1f;
    	break;
    case 'd':
    	xview += 0.1f;
    	break;
    case 'q':
    	xangle -= 0.1f;
    	break;
    case 'e':
    	xangle += 0.1f;
    	break;
  }
}

void OnTimer(int value)
{
	globalTime++;	
	glutPostRedisplay();

	//FPS-counter
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
	static GLfloat lastTick = 0;
	static GLint fps = 0;
	fps++;
	if(currentTime - lastTick > 1000) {
		std::cout << "FPS: " << fps  << std::endl;
		lastTick = currentTime;
		fps = 0;
	}

	glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow ((char*)"Grass");
	glutDisplayFunc(display);
	glutKeyboardFunc(KeyboardInput);
	init();

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(20, &OnTimer, t);

	glutMainLoop();
	return 0;
}
