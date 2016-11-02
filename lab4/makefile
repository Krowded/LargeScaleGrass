# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab4-2

lab4-2: lab4-2.c Skybox.c GrassHandler.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -std=c++11 -Wall -o lab4-2 -I$(commondir) -I../common/Linux lab4-2.c Skybox.c GrassHandler.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -DGL_GLEXT_PROTOTYPES -lm

clean :
	rm lab4-2
