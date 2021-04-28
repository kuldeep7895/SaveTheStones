/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

//----CONSTANTS-------------------------------------------------------

#define GRID_WIDTH 61
#define GRID_HEIGHT 61
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

struct Cell {
	
	float x_coor;
	float y_coor;
	float width;
	float height;
	char info;
	int color;	// 1 if black, 0 for white	

};

//----GLOBAL VARIABLES------------------------------------------------

Cell grid[GRID_WIDTH*GRID_HEIGHT];

//----FUNCTION PROTOTYPES---------------------------------------------

void ResetGrid();
int XYToIndex( int x, int y );
int IsInBounds( int x, int y );
void Visit( int x, int y );
void PrintGrid();

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        	newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

void ResetGrid()
{

	 // Fills the grid with walls ('#' characters).
	 for (int i=0; i<GRID_WIDTH*GRID_HEIGHT; ++i)
	 {

		 grid[i].info = '#';

	 }
	 
	 //PrintGrid();

}

int XYToIndex( int x, int y )
{

	 // Converts the two-dimensional index pair (x,y) into a
	 // single-dimensional index. The result is y * ROW_WIDTH + x.
	 return y * GRID_WIDTH + x;

}

int IsInBounds( int x, int y )
{

	 // Returns "true" if x and y are both in-bounds.
	 if (x < 0 || x >= GRID_WIDTH) 
	 	return false;
	
	 if (y < 0 || y >= GRID_HEIGHT) 
	 	return false;
	
	 return true;

}

// This is the recursive function

void Visit( int x, int y )
{

	 // Starting at the given index, recursively visits every direction in a
	 // randomized order.
	 // Set my current location to be an empty passage.
	 
	 grid[ XYToIndex(x,y) ].info = ' ';
	 
	 // Create an local array containing the 4 directions and shuffle their order.
	 
	 int dirs[4];
	 dirs[0] = NORTH;
	 dirs[1] = EAST;
	 dirs[2] = SOUTH;
	 dirs[3] = WEST;
	 
	 for (int i=0; i<4; ++i)
	 {
	 
		 int r = rand() & 3;
		 int temp = dirs[r];
		 dirs[r] = dirs[i];
		 dirs[i] = temp;
	 
	 }
	 
	 // Loop through every direction and attempt to Visit that direction.
	 
	 for (int i=0; i<4; ++i)
	 {
	 
		 // dx,dy are offsets from current location. Set them based
		 // on the next direction I wish to try.
		 
		 int dx=0, dy=0;
		 
		 switch (dirs[i])
		 {
		 
			 case NORTH: dy = -1; break;
			 case SOUTH: dy = 1; break;
			 case EAST: dx = 1; break;
			 case WEST: dx = -1; break;
		 
		 }

		 // Find the (x,y) coordinates of the grid cell 2 spots
		 // away in the given direction.

		 int x2 = x + (dx<<1);
		 int y2 = y + (dy<<1);

		 if (IsInBounds(x2,y2))
		 {
		
			 if (grid[ XYToIndex(x2,y2) ].info == '#')
			 {
			 
				 // (x2,y2) has not been visited yet... knock down the
				 // wall between my current position and that position
				 grid[ XYToIndex(x2-dx,y2-dy) ].info = ' ';
				 
				 //PrintGrid();
				 
				 // Recursively Visit (x2,y2)
				 Visit(x2,y2);
			 
			 }
		 
		 }
	 
	 }

}

void PrintGrid()
{

	 // Displays the finished maze to the screen.
	 
	 for (int y=0; y<GRID_HEIGHT; ++y)
	 {
		 
		 for (int x=0; x<GRID_WIDTH; ++x)
		 {
		 	
		 	//cout << grid[XYToIndex(x,y)];
		 	
		 	grid[XYToIndex(x,y)].x_coor = 45 + (SCREEN_WIDTH / 70)*x;
			grid[XYToIndex(x,y)].y_coor = 45 + (SCREEN_WIDTH / 70)*y;
			grid[XYToIndex(x,y)].width = SCREEN_WIDTH / 70;
			grid[XYToIndex(x,y)].height = SCREEN_WIDTH / 70;
		 	
		 	if(grid[XYToIndex(x,y)].info == '#')
		 	{
		 	
		 		//Render red filled quad
				SDL_Rect fillRect = { 45 + (SCREEN_WIDTH / 70)*x, 45 + (SCREEN_HEIGHT / 70)*y, SCREEN_WIDTH / 70, SCREEN_HEIGHT / 70 };  //x, y, w, h
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );		
				SDL_RenderFillRect( gRenderer, &fillRect );
				
				grid[XYToIndex(x,y)].color = 1;
		 	
		 	}
		 	
		 	else
		 	{
		 	
		 		grid[XYToIndex(x,y)].color = 0;
		 	
		 	}
		 	
		 	cout << x << " " << y << " " << grid[XYToIndex(x,y)].x_coor << " " << grid[XYToIndex(x,y)].y_coor << " " << grid[XYToIndex(x,y)].width << " " << grid[XYToIndex(x,y)].height << " " << grid[XYToIndex(x,y)].color << endl;
		 
		 }
		 
		// cout << endl;
	 
	 }
	 
//	 cout << endl;

}

int main( int argc, char* args[] )
{
	
	// Starting point and top-level control.
	srand( time(0) ); // seed random number generator.
	ResetGrid();
	Visit(1,1);
	
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				PrintGrid();

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
