#ifndef GRASSHANDLER_H
#define GRASSHANDLER_H

#include "VectorUtils3.h"

void InitGrass(mat4 projectionMatrix, GLuint numberOfTiles, vec3* tilePositions, vec3* tileNormals);
void DrawGrass(GLuint t, mat4 worldMatrix, vec3 lightVector);
void GrassDestructor();
void SetLevelsOfDetail(int levels);

#endif
