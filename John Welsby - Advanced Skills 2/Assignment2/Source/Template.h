/*
	Template.h
	John Welsby
	2009-05-13

	This legacy header declares the functions used in main.cpp
*/

#pragma once
#include "resource.h"


/***************************************/
// truevision engine control frunctions
bool CreateTVRenderWindow( char*, int, int, int );
bool InitTV3D( HWND hwnd );
void CleanTV3D();

//TODO: PUT THESE FUNCTIONS IN THE ENGINEMANAGER
void Input(float delta);	//Calculate user input in each iteration of the main loop
void PrintHUDText(); //Display coordinates and instructions


/***************************************/

