#ifndef ET_BSP_TREE_H
#define ET_BSP_TREE_H

/* Include Required Header Files */
#include "Common.h"
#include "Plane.h"
#include "Triangle.h"
#include "Sphere.h"

/* Include Ball Header, physics related */
#include "Ball.h"

/* BSP Tree node */
class BSPNode
{
public:

	/* Constructor */
	BSPNode(const Triangle &triangle, size_t allowed_children_);

	/* Destructor */
	~BSPNode();

	/* Add a triangle to this node */
	void AddTriangle(const Triangle &triangle);

	/* Render BSP Planes */
	inline void RenderPlanes(size_t child_depth)
	{
		if(child_depth == 0) return;

		/* Render this node's Plane */
		plane.Setup();

		/* Render front and back children, if they exist */
		if(front) front->RenderPlanes(child_depth - 1);
		if(back) back->RenderPlanes(child_depth - 1);
	}

	/* Render BSP Tree's Triangles */
	inline void Render()
	{
		/* Render Unmodified Triangles */
		for(size_t i(0); i < triangles.size(); i++)
		{
			glVertex3fv((GLfloat*)&triangles[i]->vertices[0]);
			glVertex3fv((GLfloat*)&triangles[i]->vertices[1]);
			glVertex3fv((GLfloat*)&triangles[i]->vertices[2]);
		}

		/* Render front and back children, if they exist */
		if(front) front->Render();
		if(back) back->Render();
	}

	/* Get all intersection data in this plane and it's children, given a sphere */
	void UpdateCollision(Ball &ball) const;

private:

	/* Give BSPTree access to BSPNode */
	friend class BSPTree;

protected:

	/* Triangles that lay on this node's plane */
	std::vector<const Triangle*> triangles;

	/*	Number of children this node is allowed to have(, to control max depth).
		If this is 0, then it is a leaf node. */
	size_t allowed_children;

	/* This node's children */
	BSPNode *front, *back;

	/* The plane of this node. */
	Plane plane;
};

/* BSP Tree */
class BSPTree
{
public:

	/* Constructor */
	BSPTree(const std::vector<Triangle> &geometry, size_t depth_ = 4);

	/* Destructor */
	~BSPTree();

	/* Get collision data, test sphere against triangles in BSP Tree */
	std::vector<IntersectionData> UpdateCollision(Ball &ball) const;

	/* Render BSP Tree Planes */
	inline void RenderPlanes(size_t child_depth = 0)
	{
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glLineWidth(2.0f);

		root->RenderPlanes(child_depth == 0? depth : child_depth);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

	/* Render BSP Tree's Triangles */
	inline void Render()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(1.0f, 1.0f, 1.0f);
		glDisable(GL_LIGHTING);

		glBegin(GL_TRIANGLES);
			root->Render();
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
	}

private:

protected:

	/* Root node of the BSP Tree. (this is the center of the tree) */
	BSPNode *root;

	/* Max depth of the BSP Tree */
	size_t depth;
};

#endif