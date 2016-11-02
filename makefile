# set this variable to the director in which you saved the common files
commondir = ./common/
srcdir = ./src/
shaderdir = ./shaders/

all : grass

grass: main.c $(srcdir)Skybox.c $(srcdir)GrassHandler.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -std=c++11 -Wall -o grass -I$(srcdir) -I$(shaderdir) -I$(commondir) -I$(commondir)Linux main.c $(srcdir)Skybox.c $(srcdir)GrassHandler.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -DGL_GLEXT_PROTOTYPES -lm

clean :
	rm grass
