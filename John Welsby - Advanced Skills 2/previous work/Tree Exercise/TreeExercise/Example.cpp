#include "stdafx.h"
#include "Example.h"

Vertex gravity(0.0f, -750.0f, 0.0f);
float rotate_speed_y(0.0f), rotate_speed_z(0.0f);
float rotate_y(0.0f), rotate_z(0.0f);

size_t render_depth(0);
bool render_bsp(true), render_bsp_planes(true);

Example::Example()
{
	/* Load Model */
	if(!model.LoadPLY("bun_zipper_res3.ply"))
	{
		DisplayMessage("Could not load model.");
		exit(-1);
	}

	model.Scale(Vertex(2500.0f, 2500.0f, 2500.0f));

	model.Setup();

	/* Create BSP Tree */
	bsp_tree = new BSPTree(model.GetTriangles(), 3);

	bsp_tree->RenderPlanes();

	/* Setup Physics Objects */
	for(size_t i(0); i < BALL_COUNT; i++)
	{
		ball[i].origin = Vertex(RandomRange(-75.0f, 75.0f), 500.0f + RandomRange(0.0f, 60.0f), RandomRange(-75.0f, 75.0f));
		ball[i].radius = RandomRange(20.0f, 30.0f);

		ball[i].SetMass(RandomRange(1.0f, 10.0f));
		ball[i].SetBounce(0.5f);

		ball[i].SetVelocity(Vertex(Vertex(RandomRange(0.0f, 10.0f), -RandomRange(0.0f, 10.0f), RandomRange(0.0f, 10.0f))));

		colour[i] = Vertex(RandomRange(), RandomRange(), RandomRange());
	}
}

Example::~Example()
{
	delete bsp_tree;
}

bool Example::Init()
{
	/* Set Background Colour */
	glClearColor(0.25f, 0.25f, 0.25f, 0.25f);

	/* Setup Lighting */
	float light_pos[] = {10.0f, 10.0f, 15.0f, 0.0f};
	float light_col[] = {0.75f, 0.75f, 0.75f, 0.0f};

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_col);
	glEnable(GL_LIGHT1);

	/* Setup the camera. */
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0f, 1.3333333f, 1.0f, 5000.0f);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0f, 500.0f, 1750.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	return true;
}

void Example::Render()
{
	/* New Matrix Stack */
	//glPushMatrix();

	/* Rotate Scene */
	/*rotate_y += rotate_speed_y * delta;
	rotate_z += rotate_speed_z * delta;*/

	//glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
	//glRotatef(rotate_z, 0.0f, 0.0f, 1.0f);

	/* Render the mesh*/
	//if(!render_bsp) model.Setup();
	///* Render the mesh using the BSP Tree's data */
	//else bsp_tree->Render();

	//model.Setup();

	/* Update Physics, using BSP Tree to speed things up*/
	for(size_t i(0); i < BALL_COUNT; i++)
	{
		/* Update the ball's position in space */
		ball[i].Update();

		/*	Update the collision/physics using the BSP Tree. */
		std::vector<IntersectionData> results = bsp_tree->UpdateCollision(ball[i]);

		/* Render the ball */
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&colour[i]);
		ball[i].Draw();
	}

	/* Render BSP Tree Planes */
	//if(render_bsp_planes) bsp_tree->RenderPlanes(render_depth);

	/* Pop matrix from the stack */
	//glPopMatrix();
}

void Example::KeyboardEvent(unsigned char key, bool pressed)
{
	switch(key) {
		case 27: /* Escape Key*/
			if(pressed) exit(0);
			break;

		case 'q': /* Toggle rendering mesh using BSP Tree data */
			if(pressed) render_bsp = !render_bsp;
			break;

		case 'e': /* Toggle rendering BSP Tree planes*/
			if(pressed) render_bsp_planes = !render_bsp_planes;
			break;

		case 'r': /* Increase rendering depth of BSP Tree planes */
			if(pressed) render_depth++;
			break;

		case 'f': /* Decrease rendering depth of BSP Tree planes */
			if(pressed && render_depth != 0) render_depth--;
			break;

		case 'w': /* Rotate scene */
			if(rotate_speed_z < 0.0f) break;

			if(pressed) rotate_speed_z = 90.0f;
			else rotate_speed_z = 0.0f;

			break;

		case 'a': /* Rotate scene */
			if(rotate_speed_y > 0.0f) break;

			if(pressed) rotate_speed_y = -90.0f;
			else rotate_speed_y = 0.0f;

			break;

		case 's': /* Rotate scene */
			if(rotate_speed_z > 0.0f) break;

			if(pressed) rotate_speed_z = -90.0f;
			else rotate_speed_z = 0.0f;

			break;

		case 'd': /* Rotate scene */
			if(rotate_speed_y < 0.0f) break;

			if(pressed) rotate_speed_y = 90.0f;
			else rotate_speed_y = 0.0f;

			break;

		/* 1 Key */
		case 49:

			/* If key has been pressed, we dont care.  We only want released. */
			if(pressed) break;

			/* Reset Physics Objects */
			for(size_t i(0); i < BALL_COUNT; i++)
			{
				ball[i].origin = Vertex(RandomRange(-75.0f, 75.0f), 500.0f + RandomRange(0.0f, 60.0f), RandomRange(-75.0f, 75.0f));
				ball[i].radius = RandomRange(20.0f, 30.0f);

				ball[i].SetMass(RandomRange(1.0f, 10.0f));
				ball[i].SetBounce(0.5f);

				ball[i].SetVelocity(Vertex(Vertex(RandomRange(0.0f, 10.0f), -RandomRange(0.0f, 10.0f), RandomRange(0.0f, 10.0f))));
				ball[i].force = Vertex();

				colour[i] = Vertex(RandomRange(), RandomRange(), RandomRange());
			}

			break;

		default: /* Report unkown keys (used for debugging) */
			std::cout << "Key: " << size_t(key) << std::endl;
			break;
	}
}
