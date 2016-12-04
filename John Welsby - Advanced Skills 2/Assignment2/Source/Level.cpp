#include "stdafx.h"
#include "Level.h"
#include "Engine.h"


void SetupLights()
{
	CTVLightEngine *pLightEngine = EngineManager::Instance()->GetLightEngine();

	int light = pLightEngine->CreateDirectionalLight( &cTV_3DVECTOR(1.f, -1.f, 1.f), 1.f, 1.f, 1.f );
}

void LoadXML(const char* pFilename, std::vector<ObjectData> &list )
{
	CTVEngine *pEngine = EngineManager::Instance()->GetEngine();
	// make sure the list is empty
	if( !list.empty() )
	{
		// delete all
		list.erase( list.begin(), list.end() );
		
	}

	// variables
	ObjectData data;
	char temp[255];
	TiXmlDocument doc(pFilename);

	// make sure the file exists and has a valid header
	if (!doc.LoadFile()) 
	{
		pEngine->AddToLog( "Unable to load xml file" );
		return;
	}

	// some more variables
	TiXmlHandle hDoc(&doc);
	TiXmlElement *pElem, *pSubElem;
	
	// get a pointer to the first element - the root
	pElem = hDoc.FirstChildElement().Element();

	// should always have a valid root but handle gracefully if it doesn't
	if (!pElem) 
		return;

	// print it out if there is one - debugging
	pEngine->AddToLog( "root" );
	pEngine->AddToLog( pElem->Value() );
	pEngine->AddToLog( "\nroot" );
	
	// get the first element	
	pElem = hDoc.FirstChildElement().FirstChildElement().Element();

	// iterate through the parent elements
	for( pElem; pElem; pElem = pElem->NextSiblingElement())
	{	
		// get the first child element
		pSubElem = TiXmlHandle(pElem).FirstChild().Element();
		bool loadData=false;
		
		// iterate through all the child elements
		for( pSubElem; pSubElem; pSubElem = pSubElem->NextSiblingElement())
		{
			// get the information
			const char *pKey = pSubElem->Value();
			const char *pText = pSubElem->GetText();
			

			// make sure it exists
			if (pKey && pText) 
			{
				// print it out - debugging
				pEngine->AddToLog( "key" );
				pEngine->AddToLog( pKey );
				pEngine->AddToLog( "text" );
				pEngine->AddToLog( pText );
				pEngine->AddToLog( "\n" );

				// store the data temporarily
				if( strcmp( pKey, "ObjectName" ) == 0 )
				{
					strcpy_s( data.name, 29, pText );
					loadData = true;
				}
				else if( strcmp( pKey, "Position" ) == 0 )
				{
					strcpy_s( temp, 254, pText );
					data.position = VectorFromString( temp );
				}
				else if( strcmp( pKey, "Rotation" ) == 0 )
				{
					strcpy_s( temp, 254, pText );
					data.orient = VectorFromString( temp );
				}	
			}
		}
		// add the data to the vector
		if (loadData)
			list.push_back( data );
	}
}


/***VectorFromString********************/
// converts a string to a TV3D vector
/***************************************/
cTV_3DVECTOR VectorFromString( char *Line )
{
	cTV_3DVECTOR temp;
	float numList[3] = {0, 0, 0};
	char numPart[15] = "              ";
	char *ptr = Line;
	int i = 0, j = 0;

	while( *ptr != '\0' )
	{		
		if( *ptr == ' ' )
		{
			numList[j] = (float)atof( numPart );
			i = 0;
			memcpy( numPart, "              ", 15 * sizeof(char) );
			++j;
		}
		else
		{
			numPart[i] = *ptr;
			i++;
		}
		ptr++;
	}

	numList[j] = (float)atof( numPart );
	memcpy( numPart, "              ", 15 * sizeof(char) );

	temp.x = numList[0];
	temp.y = numList[1];
	temp.z = numList[2];

	return temp;
}
/***************************************/



/*****LoadScene*************************/
// Load scene from the vector. 
// This is created specifically for use with
// a vector holding ObjectData structures.
// ObjectData structures are defined in TV3DCreating.h
/***************************************/
bool LoadScene( std::vector<ObjectData> list )
{
	// make sure it is not empty
	if( !list.empty() )
	{
		// then load the level
		std::vector<ObjectData>::iterator startList = list.begin();
		
		// iterate through the level data
		for( startList; startList != list.end(); startList++ )
		{
			// if we are loading a building - anything with the word building first

				// create the filename
				char buf[30];
				
				strcpy_s( buf, 29, (*startList ).name );
				strcat_s( buf, 29, ".tvm" );
				
				// create the mesh and set the details
				CTVMesh *pMesh = EngineManager::Instance()->GetScene()->CreateMeshBuilder( buf );
				
				pMesh->LoadTVM( buf );
				
				// truevision has not yet created an option for just sending a vector :(
				pMesh->SetPosition( ( *startList ).position.x, ( *startList ).position.y, ( *startList ).position.z );
				pMesh->SetRotation( ( *startList ).orient.x, ( *startList ).orient.y, ( *startList ).orient.z );
				
				int iLightMat = EngineManager::Instance()->GetMaterialFactory()->CreateLightMaterial( 0.8f, 0.8f, 0.8f, 1.f, 0.4f );		
				pMesh->SetMaterial(iLightMat);
				pMesh->SetLightingMode( cTV_LIGHTING_MANAGED );
				pMesh->SetCollisionEnable(true);
				//MeshList.push_back(pMesh);
		}
	}
	return true;
}