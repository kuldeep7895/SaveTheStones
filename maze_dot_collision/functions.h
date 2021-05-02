#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>

using namespace std;

//----CONSTANTS-------------------------------------------------------

#define GRID_WIDTH 35
#define GRID_HEIGHT 35
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

// Structure of the maze

struct Cell {
	
	SDL_Rect box;
	char info;

};

//----GLOBAL VARIABLES------------------------------------------------

extern Cell grid[GRID_WIDTH*GRID_HEIGHT];

//----FUNCTION PROTOTYPES---------------------------------------------

void ResetGrid();
int XYToIndex( int x, int y );
int IsInBounds( int x, int y );
void Visit( int x, int y );
