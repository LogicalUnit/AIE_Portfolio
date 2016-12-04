#ifndef ET_COMMON_H
#define ET_COMMON_H

/* Include windows.h, required for windows operating systems. */
#if defined(WIN32)
#	include <windows.h>
#endif

/* Include C/C++ IO Header Files. */
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <list>

/* Include the OpenGL Header File */
#include <GL/gl.h>

/* Include the OpenGL Utilities Header File */
#include <GL/glu.h>

/* Include the OpenGL Utilities Toolkit Header File */
#include "GL/glut.h"

#if defined(WIN32)
	/* Link with OpenGL Libraries */
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#	pragma comment(lib, "glut32.lib")

#endif

/* DisplayMessage function prototype */
void DisplayMessage(const char *message);

/* GetDelta function prototype */
extern float delta;

/* RandomRange function prototype */
float RandomRange(float min = 0.0f, float max = 1.0f);

#endif