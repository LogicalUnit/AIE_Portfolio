// Dijkstra.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

//The Width and Height of the Map
const unsigned int Width = 30;
const unsigned int Height = 20;

//We will be using deques for this
#include <deque>

//A 2D Point, with operators to compare them
struct C2D
{
	int X,Y;

	bool operator==( const C2D &Other)
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=( const C2D &Other)
	{
		return X != Other.X || Y != Other.Y;
	}
};


//The Actual map
//S is the Start
//E is the Target/End
//# are the walls

// !!! ---- BEWARE ---- !!!
// It is Height then Width;
// I.E Y then X
// !!! ---- BEWARE ---- !!!
char Map[Height][Width+1] = 
{	{ "              S               " },
	{ "  ################       #####" },
	{ "                 #       #    " },
	{ "######## #####################" },
	{ "   #            #             " },
	{ "   #  ######### #  #   #######" },
	{ "                   #          " },
	{ "   ########################   " },
	{ "   #                #         " },
	{ "   #                          " },
	{ "                              " },
	{ "  ################       ###  " },
	{ "                 #       #    " },
	{ "########################## ###" },
	{ "   #            #             " },
	{ "E  #  ######### #  #   #######" },
	{ "                   #          " },
	{ "   ########################   " },
	{ "   #                #         " },
	{ "   #                          " }};


//A Function to display the Map
//It clears the screen and draws the map
//You can use this 
void Display(int sleep = 0)
{
	Sleep( sleep );

	system("cls");

	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			putchar( Map[y][x] );
		}
		printf("\n");
	}

}

//This is Dijkstra algorithms
//Parameters are the Start and End's index within the graph
//The graph is a list of all accessible points (i.e points that are not a wall)
//The path will be the generated path
void Dijkstra( int Start, int End, std::deque<C2D> &Graph, std::vector<int> &Path )
{
	//Distance list, this will contain how much distance
	//it has taken to travel to this point
	//All nodes that have not been calculate yet need a
	//distance of very large value
	std::vector<float> DistMap( Graph.size(), 4e20f );

	//This will list each nodes parent, this is used to calculate the path
	std::vector<int> Previous( Graph.size(), -1 );

	//Has this node been visited yet?
	std::vector<bool> Visited( Graph.size() );

	//The very start will have no distance travelled
	DistMap[Start] = 0;

	// make sure the path is empty
	Path.clear();

	//Make a current vertex the start
	int CurrentVertex = Start;

	Visited[CurrentVertex] = true;

	unsigned int VisitedCount = 1;

	//While we have not reached the end, or exhausted all of the nodes
	while( VisitedCount < Graph.size() && CurrentVertex != End)
	{
		for (unsigned int i = 0; i < Graph.size(); i++ )
		{

			//If this node is the current node, or if the node already
			// has a parent, therefore shouldn't be calculated for again,
			//Skip this node
			if ( i == CurrentVertex || Previous[i] != -1)
				continue;

			//If this node is adjacent
			C2D current = Graph[CurrentVertex];
			C2D target = Graph[i];

			if ( abs(current.X - target.X) <= 1 && abs(current.Y - target.Y) <= 1 )
			{
				int xdiff = Graph[CurrentVertex].X - Graph[Start].X;
				int ydiff = Graph[CurrentVertex].Y - Graph[Start].Y;

				float dist = pow((float)xdiff, 2) + pow((float)ydiff, 2);
				dist = sqrt(dist);

				Previous[i] = CurrentVertex;
				DistMap[i] = dist;
			}
		}

		//Find the node with the shortest path from the start
		float Dist = 4e20f;
		int Point = 0;
		for (unsigned int i = 0; i < Graph.size(); i++)
		{
			if ( DistMap[i] < Dist && !Visited[i] )
			{
				Dist = DistMap[i];
				Point = i;
			}
		}

		//Make the new shortest path node the current node
		CurrentVertex = Point;
		Visited[Point] = true;
				
		Map[Graph[Point].Y][Graph[Point].X] = '+';
		
		Display();
		
	}

	int Point = End;

	//Work from the end, using the parents to build the path
	if ( CurrentVertex == End )
	{
		while ( Point != Start )
		{
			Path.push_back( Previous[Point] );
			Point = Previous[Point];
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//The Start and End Nodes
	C2D Start;
	C2D End;

	//Find the Start and End in the map
	for (int x = 0; x < Width; x++)
		for(int y = 0; y < Height; y++ )
		{
			if ( Map[y][x] == 'S')
			{
				Start.X = x;
				Start.Y = y;
			}

			if ( Map[y][x] == 'E')
			{
				End.X = x;
				End.Y = y;
			}
		}


	//Build the Graph from the map
	std::deque<C2D> Graph;
	
	for(int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++ )
		{
			C2D TempNode;

			TempNode.X = x;
			TempNode.Y = y;
			if ( Map[y][x] == ' ' )
				Graph.push_back( TempNode );
		}
	}

	//Add the start and end to the 
	Graph.push_back( Start );
	Graph.push_back( End );

	//The path that will store the route
	std::vector<int> Path;

	//Find the Path!!
	Dijkstra( Graph.size() - 2, Graph.size() - 1, Graph, Path );

	//Using the path, draw onto the map the found path
	for (unsigned int i = 0; i < Path.size(); i++)
	{
		if ( Path[i] != 0 )
			Map[Graph[Path[i]].Y][Graph[Path[i]].X] = '*';
	}
	Display();
	system("pause");

	//Remove the +s from the map
	for (int x = 0; x < Width; x++)
		for(int y = 0; y < Height; y++ )
		{
			if ( Map[y][x] == '+' )
				 Map[y][x] = ' ';
		}


	Display();
	system("pause");

	return 0;
}

