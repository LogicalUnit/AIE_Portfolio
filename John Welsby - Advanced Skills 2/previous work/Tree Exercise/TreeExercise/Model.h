#ifndef ET_MODEL_H
#define ET_MODEL_H

#include "Common.h"
#include "Triangle.h"

class Model
{
public:

	/* Default Constructor */
	Model();

	/* Destructor */
	~Model();

	/* Load a .ply Model File */
	bool LoadPLY(const char *filename);

	/* Scale Model */
	inline void Scale(const Vertex &scale)
	{
		for(size_t i(0); i < triangles.size(); i++)
		{
			triangles[i] = Triangle(triangles[i].vertices[0] * scale,
									triangles[i].vertices[1] * scale,
									triangles[i].vertices[2] * scale);
		}
	}

	void Setup();

	/* Render the model */
//	inline void Render() const
//	{
/*		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &triangles[0]);
		glNormalPointer(GL_FLOAT, sizeof(Vertex), &normals[0]);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)triangles.size() * 3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&colour);*/

		/*glBegin(GL_TRIANGLES);

		for(size_t i(0); i < triangles.size(); i++)
		{
			glNormal3fv((GLfloat*)&normals[i].vertices[0]);
			glVertex3fv((GLfloat*)&triangles[i].vertices[0]);
			glNormal3fv((GLfloat*)&normals[i].vertices[1]);
			glVertex3fv((GLfloat*)&triangles[i].vertices[1]);
			glNormal3fv((GLfloat*)&normals[i].vertices[2]);
			glVertex3fv((GLfloat*)&triangles[i].vertices[2]);
		}

		glEnd();*/
//	}

	/* Get Triangle Data */
	std::vector<Triangle>& GetTriangles()
	{
		return triangles;
	}

private:

protected:

	/* All the triangles in the model. */
	std::vector<Triangle> triangles;
	std::vector<Triangle> normals;

	/* Material Settings */
	Vertex colour;
};

#endif