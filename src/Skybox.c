#include "Skybox.h"
#include "MicroGlut.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"

Model *skyTop, *skyBottom, *skyLeft, *skyRight, *skyFront, *skyBack;
Model *skyboxModel;
GLuint skyboxTopTexture, skyboxBottomTexture, skyboxLeftTexture, skyboxRightTexture, skyboxFrontTexture, skyboxBackTexture;
GLuint skyboxTexture;
GLuint skyboxProgram;

GLfloat skyboxTopVertices[] = {
    -1.0f,1.0f,-1.0f,
    -1.0f,1.0f,1.0f,
     1.0f,1.0f,1.0f,
     1.0f,1.0f,-1.0f
};
GLuint skyboxTopIndices[] = { 0, 1, 2, 0, 2, 3 };

GLfloat skyboxTopTexCoords[] = { 
    0, 0, 
    0, 1, 
    1, 1,
    1, 0 };


GLfloat skyboxBottomVertices[] = {
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f
};
GLuint skyboxBottomIndices[] = { 0, 1, 2, 0, 3, 1 };

GLfloat skyboxBottomTexCoords[] = { 
    1, 0, 
    0, 1, 
    1, 1,
    0, 0 };


GLfloat skyboxLeftVertices[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f
};
GLuint skyboxLeftIndices[] = { 0, 1, 2, 0, 2, 3 };

GLfloat skyboxLeftTexCoords[] = { 
    0, 1, 
    1, 1, 
    1, 0,
    0, 0 };


GLfloat skyboxRightVertices[] = {
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, 1.0f
};
GLuint skyboxRightIndices[] = { 0, 1, 2, 0, 3, 1};

GLfloat skyboxRightTexCoords[] = { 
    1, 1, 
    0, 0, 
    1, 0,
    0, 1 };


GLfloat skyboxFrontVertices[] = {
    -1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};
GLuint skyboxFrontIndices[] = { 0, 1, 2, 0, 3, 1 };

GLfloat skyboxFrontTexCoords[] = { 
    0, 1, 
    1, 0, 
    0, 0,
    1, 1 };


GLfloat skyboxBackVertices[] = {
    -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f
};
GLuint skyboxBackIndices[] = { 0, 1, 2, 0, 3, 1 };

GLfloat skyboxBackTexCoords[] = { 
    1, 1, 
    0, 0, 
    1, 0,
    0, 1 };


//Combined versions below:
GLfloat skyboxVertices[] = {
    //Top
    -1.0f,1.0f,-1.0f,
    -1.0f,1.0f,1.0f,
     1.0f,1.0f,1.0f,
     1.0f,1.0f,-1.0f,

    //Bottom
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,

    //Left
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,

    //Right
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,

    //Front
    -1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

    //Back
    -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f
};



GLuint skyboxIndices[] = {
    //Top
    0, 1, 2, 0, 2, 3,

    //Bottom
    4, 5, 6, 4, 7, 5,

    //Left
    5, 7, 1, 5, 1, 0,

    //Right
    6, 2, 3, 6, 4, 2,

    //Front
    7, 2, 1, 7, 4, 2,

    //Back
    5, 3, 0, 5, 6, 3 
};

GLfloat skyboxTexCoords[] = {
    //Top
    0, 0, 
    0, 1, 
    1, 1,
    1, 0,

    //Bottom
    1, 0, 
    0, 1, 
    1, 1,
    0, 0,

    //Left
    0, 1, 
    1, 1, 
    1, 0,
    0, 0,

    //Right
    1, 1, 
    0, 0, 
    1, 0,
    0, 1,

    //Front
    0, 1, 
    1, 0, 
    0, 0,
    1, 1,

    //Back
    1, 1, 
    0, 0, 
    1, 0,
    0, 1,
};


//Load models, textures, shaders
void InitSkybox(mat4 projectionMatrix)
{
    skyboxProgram = loadShaders((char*)"shaders/skybox.vert", (char*)"shaders/skybox.frag");

    glUseProgram(skyboxProgram);
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "ViewToProjection"), 1, GL_TRUE, projectionMatrix.m);

    skyTop = LoadDataToModel( 
            skyboxTopVertices, 
            NULL, 
            skyboxTopTexCoords,
            NULL,
            skyboxTopIndices,
            3*2,
            6);

    skyBottom = LoadDataToModel( 
            skyboxBottomVertices, 
            NULL, 
            skyboxBottomTexCoords,
            NULL,
            skyboxBottomIndices,
            3*2,
            6);

    skyLeft = LoadDataToModel( 
            skyboxLeftVertices, 
            NULL, 
            skyboxLeftTexCoords,
            NULL,
            skyboxLeftIndices,
            3*2,
            6);
 
    skyRight = LoadDataToModel( 
            skyboxRightVertices, 
            NULL, 
            skyboxRightTexCoords,
            NULL,
            skyboxRightIndices,
            3*2,
            6);

    skyFront = LoadDataToModel( 
            skyboxFrontVertices, 
            NULL, 
            skyboxFrontTexCoords,
            NULL,
            skyboxFrontIndices,
            3*2,
            6);
    
    skyBack = LoadDataToModel( 
            skyboxBackVertices, 
            NULL, 
            skyboxBackTexCoords,
            NULL,
            skyboxBackIndices,
            3*2,
            6);



    LoadTGATextureSimple((char*)"textures/skyrender0003.tga", &skyboxTopTexture);
    LoadTGATextureSimple((char*)"textures/skyrender0006.tga", &skyboxBottomTexture);
    LoadTGATextureSimple((char*)"textures/skyrender0004.tga", &skyboxLeftTexture);
    LoadTGATextureSimple((char*)"textures/skyrender0001.tga", &skyboxRightTexture);
    LoadTGATextureSimple((char*)"textures/skyrender0005.tga", &skyboxFrontTexture);
    LoadTGATextureSimple((char*)"textures/skyrender0002.tga", &skyboxBackTexture);

    glBindTexture(GL_TEXTURE_2D, skyboxTopTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, skyboxBottomTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, skyboxLeftTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, skyboxRightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, skyboxFrontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, skyboxBackTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

//Should combine into a single draw call with GL_TEXTURE_CUBE_MAP, but too lazy
void DrawSkybox(mat4 cameraMatrix)
{
    glUseProgram(skyboxProgram);
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "WorldToView"), 1, GL_TRUE, cameraMatrix.m);

    glBindTexture(GL_TEXTURE_2D, skyboxTopTexture);
    DrawModel(skyTop, skyboxProgram, (char*)"inPosition", NULL, (char*)"inTexCoord");
    glBindTexture(GL_TEXTURE_2D, skyboxBottomTexture);
    DrawModel(skyBottom, skyboxProgram, (char*)"inPosition", NULL, (char*)"inTexCoord");
    glBindTexture(GL_TEXTURE_2D, skyboxLeftTexture);
    DrawModel(skyLeft, skyboxProgram, (char*)"inPosition", NULL, (char*)"inTexCoord");
    glBindTexture(GL_TEXTURE_2D, skyboxRightTexture);
    DrawModel(skyRight, skyboxProgram, (char*)"inPosition", NULL, (char*)"inTexCoord");
    glBindTexture(GL_TEXTURE_2D, skyboxFrontTexture);
    DrawModel(skyFront, skyboxProgram, (char*)"inPosition", NULL, (char*)"inTexCoord");
    glBindTexture(GL_TEXTURE_2D, skyboxBackTexture);
    DrawModel(skyBack, skyboxProgram, (char*)"inPosition", NULL, (char*)"inTexCoord");

}
