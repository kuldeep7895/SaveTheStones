#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <time.h>
#include <vector>
#include <sstream>

using namespace std;

//----CONSTANTS--------------------
//-----------------------------------

#define GRID_WIDTH 33
#define GRID_HEIGHT 33
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define THANOS 0
#define AVENGER 1

const int SCREEN_WIDTH = 850;
const int SCREEN_HEIGHT = 850;

enum stone {
    POWER_STONE,
    MIND_STONE,
    SOUL_STONE,
    TIME_STONE,
    SPACE_STONE,
    REALITY_STONE
};

// Structure of the maze

struct Cell {
	
	SDL_Rect box;  //x, y, w, h
	char info;

};

//----GLOBAL VARIABLES------------------------------------------------

extern Cell grid[GRID_WIDTH*GRID_HEIGHT];

//----FUNCTION PROTOTYPES---------------------------------------------

void ResetGrid();
int XYToIndex( int x, int y );
int IsInBounds( int x, int y );
void Visit( int x, int y );
void createMaze();

