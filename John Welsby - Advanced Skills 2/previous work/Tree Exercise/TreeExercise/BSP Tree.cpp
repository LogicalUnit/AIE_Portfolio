#include "stdafx.h"
#include "BSP Tree.h"

BSPNode::BSPNode(const Triangle &triangle, size_t allowed_children_): plane(triangle.GetPlane()), front(0), back(0), allowed_children(allowed_children_)
{
	triangles.push_back(&triangle);
}

BSPNode::~BSPNode()
{
	if(front) delete front;
	if(back) delete back;
}

void BSPNode::AddTriangle(const Triangle &triangle)
{
	/* If node is not a leaf node */
	if(allowed_children > 0)
	{
		/* Get orientation of triangle, in relation to the plane */
		switch(plane.Classify(triangle))
		{
			/* Infront of plane */
			case PO_Infront:
				if(front) front->AddTriangle(triangle);
				else front = new BSPNode(triangle, allowed_children - 1);
				break;

			/* Behind of plane */
			case PO_Behind:
				if(back) back->AddTriangle(triangle);
				else back = new BSPNode(triangle, allowed_children - 1);
				break;

			/* Coplanar to plane */
			case PO_Coplanar:
				triangles.pop_back();
				break;

			/* Intersecting plane */
			case PO_Intersecting:
			{
				/* Add triangle to both front and back children, if they exist */
				if(front) front->AddTriangle(triangle);
				else front = new BSPNode(triangle, allowed_children - 1);

				if(back) back->AddTriangle(triangle);
				else back = new BSPNode(triangle, allowed_children - 1);

				break;
			}
		}
	}
	/* If node is a leaf node. */
	else triangles.push_back(&triangle);
}

void BSPNode::UpdateCollision(Ball &ball) const
{
	bool compute_collision(false);

	/* If this is not a leaf node */
	if(allowed_children > 0)
	{
		/* Get the orientation of the sphere, in relation to the node's plane */
		switch(plane.Classify(ball))
		{
			/* If sphere is infront of the plane */
			case PO_Infront:

				/* Test against front child, if it exists */
				if(front) front->UpdateCollision(ball);

				break;

			/* If sphere is behind of the plane */
			case PO_Behind:

				/* Test against front back, if it exists */
				if(back) back->UpdateCollision(ball);

				break;

			/* If sphere is intersecting the plane */
			case PO_Intersecting:
			{
				/* Test intersection against children's data, if they exist */
				if(front) front->UpdateCollision(ball);
				if(back) back->UpdateCollision(ball);

				compute_collision = true;

				break;
			}
		}
	}
	/* If this is a leaf node */
	else compute_collision = true;

	if(compute_collision)
	{
		IntersectionData data;

		/* Test intersection against all triangles */
		for(size_t i(0); i < triangles.size(); i++)
		{
			/* If sphere intersects trinangle */
			if(ball.Intersect(*triangles[i], &data))
			{
				/* Displace sphere, so that it no longer intersects the triangle */
				ball.origin += data.normal * (ball.radius - data.distance);

				/*	Set velocity to the reflected value, given the incident direction (velocity)
					and the normal. */
				ball.velocity = ball.velocity - 2 * Vertex::dot(ball.velocity, data.normal) * data.normal;

				/* Scale velocity by a friction value, in this case, just the balls bounce value */
				ball.velocity *= ball.bounce;
			}
		}
	}
}

BSPTree::BSPTree(const std::vector<Triangle> &geometry, size_t depth_): root(0), depth(depth_)
{
	/* Create root node from the first triangle in the geometry */
	root = new BSPNode(geometry[0], depth);

	/* Add all triangles to the root node. */
	for(size_t i(1); i < geometry.size(); i++)
	{
		root->AddTriangle(geometry[i]);
	}
}

BSPTree::~BSPTree()
{
	/* Delete root node, if it exists */
	if(root) delete root;
}

std::vector<IntersectionData> BSPTree::UpdateCollision(Ball &ball) const
{
	std::vector<IntersectionData> results;

	root->UpdateCollision(ball);

	return results;
}