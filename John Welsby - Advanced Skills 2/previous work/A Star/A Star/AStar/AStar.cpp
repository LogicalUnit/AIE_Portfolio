// AStar.cpp : Defines the entry point for the console application.
//
#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

const unsigned int Width = 30;
const unsigned int Height = 20;

#include <deque>

//A structure for an individual node
//This contains the position data of the parent
//And the G and H values
struct C2D
{
	C2D()
	{
		X = Y = 0;
		ParentX = 0;
		ParentY = 0;
		G = 0;
		H = 0;
	}

	int X,Y;
	int ParentX, ParentY;
	int G;
	float H;

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
	{ "  ################       ###  " },
	{ "                 #       #    " },
	{ "###### #######################" },
	{ "   #            #             " },
	{ "   #  ######### #  #   #######" },
	{ "                   #          " },
	{ "###########################   " },
	{ "   #                #         " },
	{ "   #                          " },
	{ "                              " },
	{ "  ################       #####" },
	{ "                 #       #    " },
	{ "#################### ##### ###" },
	{ "   #      #      #            " },
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

//This is to estimate how far it is from a node
// you will need to implement this properly
float GetH( const C2D &Node, const C2D &End )
{
	int xdiff = End.X - Node.X;
	int ydiff = End.Y - Node.Y;

	float result = pow((float)xdiff, 2) + pow((float)xdiff, 2);
	result = sqrt(result);

	return result;
}

//Checks to see if this square is already in the lists or not
bool Check( C2D Square, std::deque<C2D> &open, std::deque<C2D> &closed)
{
	for (unsigned int i = 0; i < open.size(); i++)
	{
		if ( open[i] == Square)
			return true;
	}

	for (unsigned int i = 0; i < closed.size(); i++)
	{
		if ( closed[i] == Square)
			return true;
	}

	return false;
}

//The actual A* Algorithm
// Start and End are the Start and Ending Points
// open and closed are the 2 lists that A* uses
// The open list is all the availble nodes to search
// The closed list is all the nodes that have been searched
// Both lists start empty
// Making the algorithm Recursive makes it easier
void AStar( C2D Start, C2D End, std::deque<C2D> &open, std::deque<C2D> &closed)
{
	//Add the Start to the open list
	open.push_back(Start);

	//find the good squares around it
	for (int x = -1; x < 2; x++)
		for (int y = -1; y < 2; y++)
		{
		//Is this square within bounds of the map
			if ( (Start.Y+y) > Height-1 || (Start.Y+y) < 0 )
				continue;
			if ( (Start.X+x) > Width-1 || (Start.X+x) < 0 )
				continue;
			//Find adajcent squares that are not walls
			//Add them to the open list
			if ( Map[Start.Y+y][Start.X+x] != '#' )
			{
			// create a C2D square and fill its structure
				// with the information required
				C2D Square;
				Square.X = Start.X+x;
				Square.Y = Start.Y+y;
				Square.ParentX = Start.X;
				Square.ParentY = Start.Y;
				Square.G = Start.G + 1;
				// get H from function GetH
				Square.H = GetH(Square, End);
				
				// check it is not already in the open or closed lists
				// if it is not then add it to the open list
				if(!Check(Square, open, closed))
					open.push_back(Square);
			}
		}
	
	//Remove the Start from the open list
	std::deque<C2D> temp;
	for (std::deque<C2D>::iterator i = open.begin(); i != open.end(); i++)
	{
		if (*i != Start)
			temp.push_back(*i);
	}
	open = temp;

	//Add it to the closed list
	closed.push_back(Start);
	
	//If there are still squares in the open list
	//(We might of run out of squares to search through
	//Therefore there is no path)
	if (open.empty())
		return;
	
	//Find the smallest F value open node
		
	float F = open[0].G + open[0].H;
	int Smallest = 0;

	for (unsigned int i = 1; i < open.size(); i++)
	{
		if ( (open[i].G + open[i].H) < F )
		{
			F = (open[i].G + open[i].H);
			Smallest = i;
		}
	}
	
	//Add the Smallest to the closed
	closed.push_back(open[Smallest]);

	//Remove it from the open list
	temp.clear();
	for (std::deque<C2D>::iterator i = open.begin(); i != open.end(); i++)
	{
		if (*i != open[Smallest])
			temp.push_back(*i);
	}
	open = temp;

	//If we have just found the target then quit
	
	if (closed.back() == End)
		return;

	//Draw onto the map the node just checked
	Map[closed.back().Y][closed.back().X] = '+';

	//Update the display
	Display();

	//Call AStar again, but using the newly found node as the start
	AStar( closed.back(), End, open, closed );
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Start and end nodes
	C2D Start;
	C2D End;

	//Find them in the map
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


	//The open and closed lists
	std::deque<C2D> closed;
	std::deque<C2D> open;

	//Display the map
	Display();

	//Start A*
	AStar( Start, End, open, closed );

	
	C2D Square = End;

	while (Square.X != Start.X || Square.Y != Start.Y)
	{
		for (unsigned int i = 0; i < closed.size(); i++)
		{
			if (Square.X == closed[i].X && Square.Y == closed[i].Y)
			{
				Square.X = closed[i].ParentX;
				Square.Y = closed[i].ParentY;
				Map[Square.Y][Square.X] = '*';
				i = 0;
				break;
			}
		}
	}
	


	//!----------------------------------------------------------------!
	//!----------------------------------------------------------------!
	//
	//			YOU WILL HAVE TO GENERATE THE PATH HERE, or make AStar
	//			RETURN THE PATH
	//!----------------------------------------------------------------!
	//!----------------------------------------------------------------!

	//Update the display
	Display();

	system("pause");

	//Remove the +s
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

