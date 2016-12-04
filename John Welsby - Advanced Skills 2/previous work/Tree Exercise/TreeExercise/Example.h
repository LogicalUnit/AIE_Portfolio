#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "Common.h"
#include "Model.h"
#include "BSP Tree.h"

/* Example Class, handles initialisation, rendering, and destruction of example. */
class Example
{
public:

	/* Default Constructor */
	Example();

	/* Default Destructor */
	~Example();

	/* Initialise the example in here. */
	bool Init();

	/* Rendering function, call all rendering related functions here. */
	void Render();

	/* This is called when a key is pressed */
	void KeyboardEvent(unsigned char key, bool pressed);

private:

protected:

#	define BALL_COUNT 6

	/* Model */
	Model model;

	/* BSP Tree */
	BSPTree *bsp_tree;

	/* Ball Information */
	Vertex colour[BALL_COUNT];
	Ball ball[BALL_COUNT];
};

#endif
