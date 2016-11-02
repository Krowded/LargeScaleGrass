#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <cmath>
#include <iostream>
#include "GrassTypes.h"
#include "LoadTGA.h"
#include "GrassHandler.h"

#define LOCAL static

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
	//vec2 indices[vertNum];
	//vec2 normals[vertNum];
};

const GLfloat baseSizeModifier = 5;
const GLint maxGrassPerTile = 1000;
const GLuint totalLevelsOfDetail = 3;
const GLuint levelsOfDetail[] = { 100, 100, 0 };
const GLint totalNumberOfStraws = 160000;
const GLfloat grassMinSize = 0.01;
const GLfloat grassMaxSize = 0.05;
const GLfloat grassMinThickness = 0.01;
const GLfloat grassMinHue = 0.0;

//Array of arrays to upload
mat4* tileTransformations[totalLevelsOfDetail];
GLuint* offsets[totalLevelsOfDetail];


GLuint grassProgram;
GLuint grassTexture;
GLfloat globalTime = 0;

//One VAO for all the grass
GLuint grassVAO;
GLuint tileVBO, strawVBO;
GLuint tileTransformationLocation;

//Tiles
LOCAL GLuint totalNumberOfTiles;
Tile* Tiles;
Straw Straws[totalNumberOfStraws];

void GenerateGrass() {
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


 	glUseProgram(grassProgram);
	//Generate VAOs, VBOs
	GLuint indexBufferObjID, texCoordBufferObjID, grassNormalBuffer;
	glGenVertexArrays(1, &grassVAO);
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
    GLuint grassTransformationLocation = glGetAttribLocation(grassProgram, "modelToWorld");
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

    printError("dynamic grasstranformation setup");

	glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
	tileTransformationLocation = glGetAttribLocation(grassProgram, "tileTransformation");
	glEnableVertexAttribArray(tileTransformationLocation); 
	glEnableVertexAttribArray(tileTransformationLocation+1); 
	glEnableVertexAttribArray(tileTransformationLocation+2);
	glEnableVertexAttribArray(tileTransformationLocation+3);
	glVertexAttribPointer(tileTransformationLocation, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)0);
    glVertexAttribPointer(tileTransformationLocation+1, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(sizeof(vec4)));
    glVertexAttribPointer(tileTransformationLocation+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(2 * sizeof(vec4)));
    glVertexAttribPointer(tileTransformationLocation+3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(3 * sizeof(vec4))); 

	printError("dynamic tiletransformations setup");

    ////Static

	//Upload indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices*sizeof(GLubyte), indices, GL_STATIC_DRAW);

	printError("upload indices");

	//Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, vertNum*2*sizeof(GLfloat), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(grassProgram, "in_TexCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(grassProgram, "in_TexCoords"));

	printError("upload texture coordinates");

	glUniform2fv(glGetUniformLocation(grassProgram, "vertices"), 5*vertNum, combinedVertices);

	printError("upload position");

	glUniform2fv(glGetUniformLocation(grassProgram, "normals"), 5*vertNum, combinedNormals);

	printError("upload normal");

    glBindVertexArray(0);
}	

void GenerateTiles(vec3* tilePositions, vec3* tileNormals)
{ 
	vec3 upVector = SetVector(0,1,0);
	Tiles = (Tile*)malloc(totalNumberOfTiles*sizeof(Tile)); //TODO: Free somewhere
	for(GLuint i = 0; i < totalNumberOfTiles; ++i)
	{	
		Tiles[i].offset = rand() % (totalNumberOfStraws - maxGrassPerTile);

		//Tiles[i].position = SetVector(tilePositions[i].x, tilePositions[i].y, tilePositions[i].z);
		Tiles[i].position = SetVector(tilePositions[i].x, 0.0f, tilePositions[i].z); //Not interested in height

		//Rotate to match normal	
		vec3 rotationAxis = Normalize(CrossProduct(upVector, tileNormals[i]));

		if ( tileNormals[i].y >  0.99) //Check that the normal isn't equal to up
			rotationAxis = SetVector(1,0,0);

		//rotationAxis = SetVector(1,0,0);
		GLfloat diffAngle = acos( DotProduct(upVector, tileNormals[i]));
		mat4 rotation = ArbRotate(rotationAxis, diffAngle);
		mat4 translation = T(tilePositions[i].x, tilePositions[i].y, tilePositions[i].z);
		Tiles[i].transformation = Transpose(Mult(translation, rotation));
	}
    printError("generate tiles");

}

static mat4 projectionMatrix;
void InitGrass(mat4 projection, GLuint tileAmount, vec3* tilePositions, vec3* tileNormals)
{
	dumpInfo();

	//projectionMatrix = perspective(90, 640/480, 0.2, 10000);//windowWidth/windowHeight, nearDrawDistance, drawDistance);
	projectionMatrix = projection;
	totalNumberOfTiles = tileAmount;
	for(GLuint i = 0; i < totalLevelsOfDetail; ++i) {
		tileTransformations[i] = (mat4*)malloc(totalNumberOfTiles*sizeof(mat4));
		offsets[i] = (GLuint*)malloc(totalNumberOfTiles*sizeof(GLuint));
	}


	// GL inits
	glClearColor(0.5,0.5,0.5,0);
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	grassProgram = loadShaders("grassShader.vert", "grassShader.frag");
	printError("init grass shader");
	
	//Upload textures
	glUniform1i(glGetUniformLocation(grassProgram, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple((char*)"texture.tga", &grassTexture);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	printError("upload textures");

	// Generate geometry and VBOs GPU:
	GenerateTiles(tilePositions, tileNormals);
	GenerateGrass();

	// End of upload of geometry
	
	printError("init arrays");
}



GLfloat xview = 0.0f;
GLfloat yview = 0.0f;
GLfloat xangle = 0.0f;
void DrawGrass(GLuint globalTime, mat4 worldMatrix)
{
	glUseProgram(grassProgram);
	glBindVertexArray(grassVAO);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	//Upload time
	glUniform1f(glGetUniformLocation(grassProgram, "time"), globalTime);

	//Upload matrices
	glUniformMatrix4fv(glGetUniformLocation(grassProgram, "worldToProjection"), 1, GL_TRUE, Mult(projectionMatrix, worldMatrix).m);

	//Light vectors
	vec3 upVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(0,1, -1));
	glUniform3fv(glGetUniformLocation(grassProgram, "up"), 1, &upVector.x);
	vec3 lightVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(1,1,0));
	glUniform3fv(glGetUniformLocation(grassProgram, "light"), 1, &lightVector.x);


	//Set tileTransformation vertex attribute array	
	GLuint strawCount = 0;
	//GLuint offsets[totalLevelsOfDetail][totalNumberOfTiles];
	GLuint tilesToProcess[totalLevelsOfDetail] = {0};
	//mat4 tileTransformations[totalLevelsOfDetail][totalNumberOfTiles];

	mat4 inverseWorld = InvertMat4(worldMatrix);
	vec3 cameraPosition = SetVector((inverseWorld.m)[3], 0.0f, (inverseWorld.m)[11]);
	for(GLuint i = 0; i < totalNumberOfTiles; ++i) { //TODO: Frustumculling, distance sorting
		
		GLfloat distanceToTile = Norm(VectorSub(cameraPosition, Tiles[i].position));

		GLuint level;
		if(distanceToTile < 20.0f)
		{
			level = 0;
		} else if (distanceToTile < 40.0f) {
			level = 1;
		} else {
			level = 2;
		}
		

		tileTransformations[level][tilesToProcess[level]] = Tiles[i].transformation;
		offsets[level][tilesToProcess[level]] = Tiles[i].offset;
		tilesToProcess[level]++;
	}

	//Sort by distance for every level of detail
	for(GLuint i = 0; i < totalLevelsOfDetail; ++i) {

		GLuint strawsPerTile = levelsOfDetail[i];
		GLuint numOfTilesToDraw = tilesToProcess[i];
		GLuint totalInstances = numOfTilesToDraw*strawsPerTile;

		if(totalInstances < 1)
			continue;
		
		//Tile (any way to skip this step?)
		glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
		glBufferData(GL_ARRAY_BUFFER, numOfTilesToDraw * sizeof(mat4), tileTransformations[i], GL_STREAM_DRAW);
		glVertexAttribDivisor(tileTransformationLocation, strawsPerTile);
		glVertexAttribDivisor(tileTransformationLocation+1, strawsPerTile);
		glVertexAttribDivisor(tileTransformationLocation+2, strawsPerTile);
		glVertexAttribDivisor(tileTransformationLocation+3, strawsPerTile);

/*		//Needed for wind?
		glBindBuffer(GL_ARRAY_BUFFER, tileHeightBO);
		glBufferData(GL_ARRAY_BUFFER, totalNumberOfTiles * sizeof(GLfloat), tileHeights, GL_STREAM_DRAW);
		glVertexAttribDivisor(tileTransformationLocation, strawsPerTile);
*/
		
		//Grass Transformations
		glBindBuffer(GL_ARRAY_BUFFER, strawVBO);
		glBufferData(GL_ARRAY_BUFFER, totalInstances * sizeof(Straw), NULL, GL_DYNAMIC_DRAW);
		for(GLuint j = 0; j < numOfTilesToDraw; ++j) 
			glBufferSubData(GL_ARRAY_BUFFER, j*strawsPerTile*sizeof(Straw), strawsPerTile * sizeof(Straw), &Straws[offsets[i][j]]);

		//Draw all the straws at this LOD
	    glDrawElementsInstancedBaseInstance( GL_TRIANGLES, numOfIndices, GL_UNSIGNED_BYTE, 0L, totalInstances, 0);
	    strawCount += totalInstances;
	}

	//FPS
	if(int(globalTime) % 1000 < 1)
		std::cout << "Drawing " << strawCount  << " straws" << std::endl;
}

void Destructor() {
	free(Tiles);
	for(GLuint i = 0; i < totalLevelsOfDetail; ++i) {
		free(tileTransformations[i]);
		free(offsets[i]);
	}
}