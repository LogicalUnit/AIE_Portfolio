#include "StdAfx.h"

GameProcessing::GameProcessing(void)
{
	// server defines the number, starting positions
	// and initial velocities of all asteroids.
	// Server defines the starting positions of all clients
	// server defines the start of the game and controls dt
	//server broadcasts names, scores and death messages.
	// clients handle own scores and lives but informs server of changes
}

GameProcessing::~GameProcessing(void)
{
}

void GameProcessing::Update( float dt )
{
	//game state - Waiting for challenger ( minimum 2 )
	//			 - Game initialising - sending asteroid starting data and player starting position
	//			 - game playing
	
	//process any collision messages that have occurred from clients - 
			//asteroid to self or own bullet to enemy or asteroid
			// update any change in asteroid positions/state as a result of collision
			// update any score as a result of collision
	/*
	ie client handles own collision detection and response.
	client gets new position and velocity of asteroids collided with and tells server
	Server then broadcasts it to all clients and they update their asteroid position and velocities

	client calculates own score and informs the server that there has been a change
	 - the reason ( eg.asteroid collision ) and new score or death.
	 server broadcasts new score for player to all players and message like 
	 "player 3 obliterated themselves on an asteroid )
			
	if client bullet hits another player or asteroid then they destroy the bullet, 
	change the state of the asteroid and tell the server the result - like the asteroid's
	new velocity or the player's id that was hit.
	The server tells the player they were hit

	client tells server every update - new position and facing
	server tells client every update - new position and facing of other players
	
	// need id of every enemy and asteroid and bullet from own ship
	// client uses a prediction for the next enemy position - server confirms it
	*/
}