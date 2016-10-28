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

struct Straw {
	mat4 transformation;
	GLfloat thickness;
	GLfloat hue;
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
const GLint maxGrassPerTile = 5000;
const GLint totalNumberOfStraws = 160000;
const GLfloat grassMinSize = 0.01;
const GLfloat grassMaxSize = 0.05;
const GLfloat grassMinThickness = 0.01;
const GLfloat grassMinHue = 0.1;

GLuint grassShader;
GLuint grassTexture;
GLfloat globalTime = 0;

//One VAO for all the grass
GLuint grassVAO;
GLuint tileVBO, strawVBO;
GLuint tileTransformationLocation;

//Tiles
const GLuint numberOfTiles = 10;
const GLuint maxTiles = 1000;
mat4 tileTransformations[maxTiles];
Tile* Tiles;
Straw Straws[totalNumberOfStraws];

void generateGrass() {
	//Procedural generation of the grass
	srand(0);
	for(int i = 0; i < totalNumberOfStraws; ++i) {
		GLfloat length = (float(rand() % 1000*grassMaxSize)/1000.0f + grassMinSize) * baseSizeModifier;
		GLfloat angle = M_PI * float(rand() % 1000)/500.0f;
		GLfloat displacementx = tileSize * float(rand() % 1000)/1000.0f;
		GLfloat displacementz = tileSize * float(rand() % 1000)/1000.0f;

		mat4 scaling = S(length, length, length);
		mat4 rotation = Ry(angle);
		mat4 translation = T(displacementx, 0, displacementz);
		Straws[i].transformation = Transpose(Mult(translation, Mult(rotation, scaling)));

		Straws[i].thickness = (rand() % 1500)/10000.0f + grassMinThickness;
		Straws[i].hue = float((rand() % 1500))/1000.0f + grassMinHue;
 	}

	//Generate VAOs, VBOs
	GLuint indexBufferObjID, texCoordBufferObjID, grassVertexBuffer, grassNormalBuffer;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVertexBuffer);
	glGenBuffers(1, &grassNormalBuffer);
	glGenBuffers(1, &indexBufferObjID);
	glGenBuffers(1, &texCoordBufferObjID);
	glGenBuffers(1, &tileVBO);
	glGenBuffers(1, &strawVBO);

	GLuint VAO = grassVAO;
	glBindVertexArray(VAO);

	//Set up VBOs and upload the static ones

	////Dynamic

	glBindBuffer(GL_ARRAY_BUFFER, strawVBO);
    GLuint grassTransformationLocation = glGetAttribLocation(grassShader, "modelToWorld");
    glEnableVertexAttribArray(grassTransformationLocation); 
    glEnableVertexAttribArray(grassTransformationLocation+1);
    glEnableVertexAttribArray(grassTransformationLocation+2);
    glEnableVertexAttribArray(grassTransformationLocation+3);
    glEnableVertexAttribArray(grassTransformationLocation+4);
    glEnableVertexAttribArray(grassTransformationLocation+5);
    glVertexAttribPointer(grassTransformationLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Straw), (GLvoid*)0);
    glVertexAttribPointer(grassTransformationLocation+1, 4, GL_FLOAT, GL_FALSE, sizeof(Straw), (GLvoid*)(sizeof(vec4)));
    glVertexAttribPointer(grassTransformationLocation+2, 4, GL_FLOAT, GL_FALSE, sizeof(Straw), (GLvoid*)(2 * sizeof(vec4)));
    glVertexAttribPointer(grassTransformationLocation+3, 4, GL_FLOAT, GL_FALSE, sizeof(Straw), (GLvoid*)(3 * sizeof(vec4)));
    glVertexAttribPointer(grassTransformationLocation+4, 1, GL_FLOAT, GL_FALSE, sizeof(Straw), (GLvoid*)(sizeof(mat4)));
    glVertexAttribPointer(grassTransformationLocation+5, 1, GL_FLOAT, GL_FALSE, sizeof(Straw), (GLvoid*)(sizeof(mat4) + sizeof(GLfloat)));
    glVertexAttribDivisor(grassTransformationLocation, 1);
    glVertexAttribDivisor(grassTransformationLocation+1, 1);
    glVertexAttribDivisor(grassTransformationLocation+2, 1);
    glVertexAttribDivisor(grassTransformationLocation+3, 1);
    glVertexAttribDivisor(grassTransformationLocation+4, 1);
    glVertexAttribDivisor(grassTransformationLocation+5, 1);

    printError("dynamic vbo setup");

    ////Static

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

	glUniform2fv(glGetUniformLocation(grassShader, "vertices"), 5*vertNum, combinedVertices);

	printError("upload position");

	glUniform2fv(glGetUniformLocation(grassShader, "normals"), 5*vertNum, combinedNormals);

	printError("upload normal");

	glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
	tileTransformationLocation = glGetAttribLocation(grassShader, "tileTransformation");
	glEnableVertexAttribArray(tileTransformationLocation); 
	glEnableVertexAttribArray(tileTransformationLocation+1); 
	glEnableVertexAttribArray(tileTransformationLocation+2);
	glEnableVertexAttribArray(tileTransformationLocation+3);
	glVertexAttribPointer(tileTransformationLocation, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)0);
    glVertexAttribPointer(tileTransformationLocation+1, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(sizeof(vec4)));
    glVertexAttribPointer(tileTransformationLocation+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(2 * sizeof(vec4)));
    glVertexAttribPointer(tileTransformationLocation+3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(3 * sizeof(vec4))); 

	printError("enabling tileTransformations");

    glBindVertexArray(0);
}	

void generateTiles() {
	Tiles = (Tile*)malloc(numberOfTiles*sizeof(Tile)); //TODO: Free somewhere
	for(int i = 0; i < numberOfTiles; ++i)
	{	
		Tiles[i].position = SetVector(tilePositions[i*3], tilePositions[i*3+1], tilePositions[i*3+2]);
		Tiles[i].transformation = Transpose(T( tilePositions[i*3], tilePositions[i*3+1], tilePositions[i*3+2]));
		Tiles[i].offset = rand() % (totalNumberOfStraws - maxGrassPerTile);
	}
    printError("generate tiles");

}

mat4 projectionMatrix;
void init(void)
{
	dumpInfo();

	projectionMatrix = perspective(90, 640/480, 0.2, 10000);//windowWidth/windowHeight, nearDrawDistance, drawDistance);


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
	generateTiles();
	generateGrass();

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
	//worldMatrix = T(-xview,-yview,0);

	worldMatrix = Mult(Rx(xangle), worldMatrix);
	projectionMatrix =  IdentityMatrix();

	//Upload matrices
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "worldToProjection"), 1, GL_TRUE, Mult(projectionMatrix, worldMatrix).m);

	//Light vectors
	vec3 upVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(0,1, -1));
	glUniform3fv(glGetUniformLocation(grassShader, "up"), 1, &upVector.x);
	vec3 lightVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(1,1,0));
	glUniform3fv(glGetUniformLocation(grassShader, "light"), 1, &lightVector.x);

	//Time
	glUniform1f(glGetUniformLocation(grassShader, "time"), globalTime);

	///Draw the grass
	glDisable(GL_CULL_FACE); //Both sides need to be visible
	glBindVertexArray(grassVAO);
	//Set tileTransformation vertex attribute array
	GLuint totalLevelsOfDetail = 1;
	GLuint levelOfDetail[] = { maxGrassPerTile };
	GLuint strawCount = 0;
	GLuint offsets[numberOfTiles];
	GLuint numberOfTilesToProcess = 0;
	//Sort by distance for every level of detail
	for(int i = 0; i < totalLevelsOfDetail; ++i) {

		for(int j = 0; j < numberOfTiles; ++j) { //TODO: Check distance and sort
			tileTransformations[j] = Tiles[j].transformation;
			offsets[j] = Tiles[j].offset;
		}

		GLuint strawsPerTile = levelOfDetail[i];
		GLuint totalInstances = numberOfTiles*strawsPerTile;
		
		if(numberOfTiles < 1)
			continue; 

		//Tile (anyway to skip this step?)
		glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
		glBufferData(GL_ARRAY_BUFFER, numberOfTiles * sizeof(mat4), tileTransformations, GL_STATIC_DRAW);
		glVertexAttribDivisor(tileTransformationLocation, strawsPerTile);
		glVertexAttribDivisor(tileTransformationLocation+1, strawsPerTile);
		glVertexAttribDivisor(tileTransformationLocation+2, strawsPerTile);
		glVertexAttribDivisor(tileTransformationLocation+3, strawsPerTile);
		
		//Grass Transformations
		glBindBuffer(GL_ARRAY_BUFFER, strawVBO);
		glBufferData(GL_ARRAY_BUFFER, totalInstances * sizeof(Straw), NULL, GL_STATIC_DRAW);
		for(int j = 0; j < numberOfTiles; ++j) 
			glBufferSubData(GL_ARRAY_BUFFER, j*strawsPerTile*sizeof(Straw), strawsPerTile * sizeof(Straw), &Straws[offsets[j]]);

		//Draw all the straws at this LOD
	    glDrawElementsInstancedBaseInstance( GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0L, totalInstances, 0);
	    strawCount += totalInstances;
	}

	//FPS
	if(int(globalTime) % 1000 < 1)
		std::cout << "Drawing " << strawCount  << " straws" << std::endl;

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
