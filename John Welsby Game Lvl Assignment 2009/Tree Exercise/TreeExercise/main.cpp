#include "stdafx.h"
#include "Example.h"

Example *example;
float delta;

/* Simple error displaying function. */
void DisplayMessage(const char *message) {
#if defined(WIN32)
	MessageBox(GetActiveWindow(), "Error", message, MB_OK);
	//OutputDebugStr(message);
#endif

	std::cout << message << std::endl;
}

float RandomRange(float min, float max)
{
	return min + (float(rand() % 10000) / 10000.0f) * (max - min);
}

/* Keyboard function. */
static void KeyboardDown(unsigned char key, int x, int y)
{
	example->KeyboardEvent(key, true);
}

static void KeyboardUp(unsigned char key, int x, int y)
{
	example->KeyboardEvent(key, false);
}

/* Rendering function, this is where you draw everything. */
static void Render() {
	static unsigned long a = GetTickCount(), b;

	/* Calculate Delta */
	b = a;
	a = GetTickCount();

	delta = (a - b) * 0.001f;

	/* Clear OpenGL Buffers. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Call User's Render function. */
	example->Render();

	/* Swap the OpenGL backbuffer. */
	glutSwapBuffers();

	/* Continue the Loop */
	glutPostRedisplay();
}

//int main(int argc, char **argv) {
//	/* Create the window. */
//	glutInit(&argc, argv);
//	glutInitWindowSize(800, 600);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutCreateWindow("BSP Tree Example");
//	glutDisplayFunc(Render);
//	glutKeyboardFunc(KeyboardDown);
//	glutKeyboardUpFunc(KeyboardUp);
//	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
//
//	/* Initialise Application Specific Information */
//	example = new Example;
//
//	/* Init OpenGL */
//	glEnable(GL_DEPTH_TEST);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
//
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//
//	if(!example->Init())
//	{
//		DisplayMessage("Error: Could not initialize example.");
//		delete example;
//		return 1;
//	}
//
//	/* Start up the main loop. */
//	glutMainLoop();
//
//	/* Destroy Example */
//	delete example;
//}
