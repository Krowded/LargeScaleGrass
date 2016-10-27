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
const GLint numberOfBlades = 16000;
const GLfloat grassMinSize = 0.002;
const GLfloat grassMaxSize = 0.05;
const GLfloat grassMinThickness = 0.01;

GLuint grassShader;
GLuint grassTexture;
GLfloat globalTime = 0;

//Transformations
GLuint grassTransformationBuffer, grassHueBuffer, grassThicknessBuffer;

//One VAO for each type of grass
GLuint grassVAOs[5];

//Models
GLuint grassVertexBuffers[5];
GLuint grassNormalBuffers[5];

//Tiles
GLuint numberOfTiles = 18;
Tile* Tiles;


mat4 grassTransformations[numberOfBlades];
GLfloat thickness[numberOfBlades];
GLfloat hue[numberOfBlades];
GLuint textureType[numberOfBlades];


void generateGrass() {
	GLfloat types[5][64];
	for(int i = 0; i < 64; ++i) {
		types[0][i] = Type1[i];
		types[1][i] = Type2[i];
		types[2][i] = Type3[i];
		types[3][i] = Type4[i];
		types[4][i] = Type5[i];
	}

	GLfloat normals[5][64];
	for(int i = 0; i < 64; ++i) {
		normals[0][i] = Type1Normals[i];
		normals[1][i] = Type2Normals[i];
		normals[2][i] = Type3Normals[i];
		normals[3][i] = Type4Normals[i];
		normals[4][i] = Type5Normals[i];
	}
	//Procedural generation of the grass
	srand(0);
	for(int i = 0; i < numberOfBlades; ++i) {
		GLfloat length = (float(rand() % 1000*grassMaxSize)/1000.0f + grassMinSize) * baseSizeModifier;
		GLfloat angle = M_PI * float(rand() % 1000)/500.0f;
		GLfloat displacementx = tileSize * float(rand() % 1000)/1000.0f;
		GLfloat displacementz = tileSize * float(rand() % 1000)/1000.0f;

		mat4 scaling = S(length, length, length);
		mat4 rotation = Ry(angle);
		mat4 translation = T(displacementx, 0, displacementz);	
		grassTransformations[i] = Transpose(Mult(translation, Mult(rotation, scaling)));


		thickness[i] = (rand() % 1500)/10000.0f + grassMinThickness;
		hue[i] = float((rand() % 1000))/1000.0f + 0.1;
 	}

	//Generate VAOs, VBOs
	glGenVertexArrays(5, grassVAOs);
	glGenBuffers(5, grassVertexBuffers);
	glGenBuffers(5, grassNormalBuffers);
	GLuint indexBufferObjID, texCoordBufferObjID;
	glGenBuffers(1, &indexBufferObjID);
	glGenBuffers(1, &texCoordBufferObjID);
    glGenBuffers(1, &grassTransformationBuffer);
    glGenBuffers(1, &grassHueBuffer);
    glGenBuffers(1, &grassThicknessBuffer);



    // Set transformation matrices as an instance vertex attribute (with divisor 1)
    // NOTE: We're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // Normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    for(GLuint i = 0; i < 5; i++)
    {
    	GLint offset = rand() % (numberOfBlades - maxGrassPerTile); //To start in different places for different blades. Clarify!

        GLuint VAO = grassVAOs[i];
        glBindVertexArray(VAO);
    	glBindBuffer(GL_ARRAY_BUFFER, grassTransformationBuffer);
    	glBufferData(GL_ARRAY_BUFFER, numberOfBlades * sizeof(mat4), grassTransformations, GL_STATIC_DRAW);

        // Set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(glGetAttribLocation(grassShader, "model")); 
        glVertexAttribPointer(glGetAttribLocation(grassShader, "model"), 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(0 + offset*sizeof(mat4)));
        glEnableVertexAttribArray(glGetAttribLocation(grassShader, "model")+1); 
        glVertexAttribPointer(glGetAttribLocation(grassShader, "model")+1, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(sizeof(vec4) + offset*sizeof(mat4)));
        glEnableVertexAttribArray(glGetAttribLocation(grassShader, "model")+2); 
        glVertexAttribPointer(glGetAttribLocation(grassShader, "model")+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(2 * sizeof(vec4) + offset*sizeof(mat4)));
        glEnableVertexAttribArray(glGetAttribLocation(grassShader, "model")+3); 
        glVertexAttribPointer(glGetAttribLocation(grassShader, "model")+3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)(3 * sizeof(vec4) + offset*sizeof(mat4)));

        glVertexAttribDivisor(glGetAttribLocation(grassShader, "model"), 1);
        glVertexAttribDivisor(glGetAttribLocation(grassShader, "model")+1, 1);
        glVertexAttribDivisor(glGetAttribLocation(grassShader, "model")+2, 1);
        glVertexAttribDivisor(glGetAttribLocation(grassShader, "model")+3, 1);

        printError("upload model");

        glBindBuffer(GL_ARRAY_BUFFER, grassHueBuffer);
    	glBufferData(GL_ARRAY_BUFFER, numberOfBlades * sizeof(GLfloat), hue, GL_STATIC_DRAW);
    	glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_Hue")); 
        glVertexAttribPointer(glGetAttribLocation(grassShader, "in_Hue"), 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)(offset*sizeof(GLfloat)));
        glVertexAttribDivisor(glGetAttribLocation(grassShader, "in_Hue"), 1);

        printError("upload hues");

		glBindBuffer(GL_ARRAY_BUFFER, grassThicknessBuffer);
    	glBufferData(GL_ARRAY_BUFFER, numberOfBlades * sizeof(GLfloat), thickness, GL_STATIC_DRAW);
    	glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_Thickness")); 
        glVertexAttribPointer(glGetAttribLocation(grassShader, "in_Thickness"), 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)(offset*sizeof(GLfloat)));
        glVertexAttribDivisor(glGetAttribLocation(grassShader, "in_Thickness"), 1);

        printError("upload thickness");

		// VBO for vertex data
		glBindBuffer(GL_ARRAY_BUFFER, grassVertexBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, vertNum*2*sizeof(GLfloat), types[i], GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(grassShader, "in_Position"), 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_Position"));

		printError("upload position");

		//VBO for normals
		glBindBuffer(GL_ARRAY_BUFFER, grassNormalBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, vertNum*2*sizeof(GLfloat), normals[i], GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(grassShader, "in_Normal"), 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_Normal"));

		printError("upload normal");

		//Upload indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);

		printError("upload indices");


		//Texture coordinates
		glVertexAttribPointer(glGetAttribLocation(grassShader, "in_TexCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(glGetAttribLocation(grassShader, "in_TexCoords"));
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, vertNum*2*sizeof(GLfloat), textureCoordinates, GL_STATIC_DRAW);

		printError("upload texture coordinates");


        glBindVertexArray(0);
    }	
}

void generateTiles() {
	Tiles = (Tile*)malloc(numberOfTiles*sizeof(Tile));
	for(int i = 0; i < numberOfTiles; ++i)
	{
		Tiles[i].position = ((vec3*)tilePositions)[i];
		Tiles[i].offset = rand() % (numberOfBlades- maxGrassPerTile*5);
	}
	
}

/* //Helper function for creating blades
void calculateNormals() {
	 GLfloat *blade = &Type5[0];

	 //First normal == -x
	 std::cout << -1.0f << " " << 0.0f << " " << 0.0f << std::endl;

	 for(int i = 1; i < 15; ++i) 
	 {
	 	vec3 vertex0 = vec3(blade[6*(i-1)], blade[6*(i-1)+1], blade[6*(i-1)+2]);
	 	vec3 vertex1 = vec3(blade[6*i], blade[6*i+1], blade[6*i+2]);
	 	vec3 vertex2 = vec3(blade[6*(i+1)], blade[6*(i+1)+1], blade[6*(i+1)+2]);
	 	vec3 vecBefore = VectorSub(vertex0, vertex1);
	 	vec3 vecAfter = VectorSub(vertex1, vertex2);
	 	vec3 otherway =  vec3(0,0,1);

	 	vec3 normalBefore = Normalize(CrossProduct(vecBefore, otherway));
	 	vec3 normalAfter  = Normalize(CrossProduct(vecAfter, otherway));
	 	vec3 normal = Normalize(ScalarMult(VectorAdd(normalBefore, normalAfter),2.0));

	 	std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
	 }

	 	//Tip
	 	vec3 vertex0 = vec3(blade[6*15], blade[6*15+1], blade[6*15+2]);
	 	vec3 vertex1 = vec3(blade[6*16], blade[6*16+1], blade[6*16+2]);
	 	vec3 vecBetween = VectorSub(vertex0, vertex1);
	 	vec3 otherway =  vec3(0,0,1);
	 	vec3 normal = Normalize(CrossProduct(vecBetween, otherway));
	 	std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;	 
} */



void init(void)
{

	for(int i = 0; i < 32; i++)
		std::cout << textureCoordinates[i*2] << textureCoordinates[i*2+1] << std::endl;
	/* //For combining models...
	int increment = 128;
	std::cout << std::endl;
	for(int i = 0; i < 15; ++i) {
		std::cout << indices[6*i]+increment << ", " << indices[6*i+1]+increment << ", " << indices[6*i+2]+increment << "," << std::endl;
		std::cout << indices[6*i+3]+increment << ", " << indices[6*i+4]+increment << ", " << indices[6*i+5]+increment << "," << std::endl;
		std::cout << std::endl;
	}*/



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
	LoadTGATextureSimple("texture.tga", &grassTexture);
	glBindTexture(GL_TEXTURE_2D, grassTexture);

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
		cos(globalTime/100.0f), 0.0f, -sin(globalTime/100.0f), -xview,
		0.0f,                1.0f, 0.0f,                 -yview,
		sin(globalTime/100.0f), 0.0f, cos(globalTime/100.0f),  0.0f,
		0.0f,                0.0f, 0.0f,                 1.0f 
	};

//	worldMatrix = T(-xview,-yview,0);

	worldMatrix = Mult(Rx(xangle), worldMatrix);

	mat4 projectionMatrix = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};
	
	//Upload matrices
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "world"), 1, GL_TRUE, worldMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "projection"), 1, GL_TRUE, projectionMatrix.m);

	//Light vectors
	vec3 upVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(0,1,0));
	glUniform3fv(glGetUniformLocation(grassShader, "up"), 1, &upVector.x);
	vec3 lightVector = MultMat3Vec3(mat4tomat3(worldMatrix), SetVector(1,1,0));
	glUniform3fv(glGetUniformLocation(grassShader, "light"), 1, &lightVector.x);

	//Time
	glUniform1f(glGetUniformLocation(grassShader, "time"), globalTime);

	//Tile position
	mat4 tileTransformation = T(-0.5,0,0);
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "tileTransformation"), 1, GL_TRUE, tileTransformation.m);


	//Draw the grass
	glDisable(GL_CULL_FACE); //Both sides need to be visible
	for(int j = 0; j < numberOfTiles; ++j)
	{	
		mat4 tileTransformation = T(Tiles[j].position.x,Tiles[j].position.y,Tiles[j].position.z);
		glUniformMatrix4fv(glGetUniformLocation(grassShader, "tileTransformation"), 1, GL_TRUE, tileTransformation.m);
		for(int i = 0; i < 5; ++i)
		{
			glBindVertexArray(grassVAOs[i]);
			//Draw an instanced number of blades, taking transformations from a random spot in grassTransformations[]
		    glDrawElementsInstancedBaseVertexBaseInstance( GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0L, maxGrassPerTile, 0, Tiles[i].offset + i*maxGrassPerTile);
		    glBindVertexArray(0);
		}
	}

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
