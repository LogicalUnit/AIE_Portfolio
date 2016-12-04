#include "stdafx.h"
#include "Model.h"
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable : 4996)



Model::Model(): colour(1.0f, 1.0f, 1.0f)
{
}

Model::~Model()
{
}

/* Warning:

	For simplicity, this PLY loader assumes the following element and property layout:
		element vertex ...
		property float x
		property float y
		property float z
		property float ...
		element face ...
		property list uchar int vertex_indices	# With 3 indices per face.
		...
*/
bool Model::LoadPLY(const char *filename)
{
	std::fstream stream(filename, std::ios::binary | std::ios::in);
	if(stream.fail()) return false;
	else
	{
		/* Vertices */
		std::vector<Vertex> vertex_data;

		/* Parsing Variables */
		size_t vertices(0), faces(0), indices[3];
		char buffer[2048];
		const char *index;
		bool EOH(false);

		/* Get the number of vertices and faces */
		while(!EOH || vertices == 0 || faces == 0)
		{
			stream.getline(&buffer[0], 2048);
			if(strstr(buffer, "element"))
			{
				index = strstr(buffer, "vertex");
				if(index)
				{
					vertices = atoi(strstr(index, " "));

					std::cout << "Vertices: " << unsigned int(vertices) << std::endl;
					continue;
				}

				index = strstr(buffer, "face");
				if(index)
				{
					faces = atoi(strstr(index, " "));

					std::cout << "Faces: " << unsigned int(faces) << std::endl;
					continue;
				}

				std::cout << "Unkown Element." << std::endl;
			}
			else if(strstr(buffer, "end_header")) EOH = true;
		}

		vertex_data.resize(vertices);
		triangles.resize(faces);
		normals.resize(faces);

		/* Read in vertices */
		for(size_t i(0); i < vertices; i++)
		{
			stream.getline(&buffer[0], 2048);

			Vertex &vertex = vertex_data[i];

			index = strtok(buffer, " ");
			vertex_data[i].x = atof(index);

			index = strtok(NULL, " ");
			vertex_data[i].y = atof(index);

			index = strtok(NULL, " ");
			vertex_data[i].z = atof(index);
		}

		/* Read in indices */
		for(size_t i(0); i < faces; i++)
		{
			stream.getline(&buffer[0], 2048);

			index = strtok(buffer, " ");
			index = strtok(NULL, " ");
			indices[0] = atoi(index);

			index = strtok(NULL, " ");
			indices[1] = atoi(index);

			index = strtok(NULL, " ");
			indices[2] = atoi(index);

			triangles[i] = Triangle(vertex_data[indices[0]], vertex_data[indices[1]], vertex_data[indices[2]]);
		}

		/* Generate Normal here... for simplicities sake (as we're storing the vertices without indices)
			Slow, but the easiest way to do it given the way we store our vertex data. */
		for(size_t i(0); i < triangles.size(); i++)
		{
			Vertex normal(triangles[i].GetNormal());

			normals[i].vertices[0] += normal;
			normals[i].vertices[1] += normal;
			normals[i].vertices[2] += normal;

			/*for(size_t j(0); j < triangles.size(); j++)
			{
				for(char k(0); k < 3; k++)
				{
					for(char l(0); l < 3; l++)
					{
						if(triangles[i].vertices[k] == triangles[j].vertices[l])
						{
							normals[j].vertices[l] += normal;
						}
					}
				}
			}*/
		}

		for(size_t i(0); i < triangles.size(); i++)
		{
			normals[i].vertices[0] = Vertex::normalized(normals[i].vertices[0]);
			normals[i].vertices[1] = Vertex::normalized(normals[i].vertices[1]);
			normals[i].vertices[2] = Vertex::normalized(normals[i].vertices[2]);
		}
	}

	stream.close();

	return true;
}

void Model::Setup()
{

	for(size_t i(0); i < triangles.size(); i++)
	{
		CTVMesh *mesh = EngineManager::Instance()->GetScene()->CreateMeshBuilder();


		mesh->AddTriangle( 0,
						triangles[i].vertices[0].x,
						triangles[i].vertices[0].y,
						triangles[i].vertices[0].z,
						triangles[i].vertices[1].x,
						triangles[i].vertices[1].y,
						triangles[i].vertices[1].z,
						triangles[i].vertices[2].x,
						triangles[i].vertices[2].y,
						triangles[i].vertices[2].z );


		CTVMaterialFactory *pMat = EngineManager::Instance()->GetMaterialFactory();
		int mat = pMat->CreateMaterialQuick(0.0f, 0.0f, 1.0f, 1.0f, "bunny");
		mesh->SetMaterial(mat);
		mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
		mesh->SetAlphaTest(false, 128, false); 
		mesh->SetBlendingMode( cTV_BLEND_ADDALPHA );

	}
}