//Purpy Pupple's amazing maze generator. 
//Released under the CC-BY-SA 3.0 License and the GFDL
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <iostream>
using namespace std;
#define UP 0     //-y
#define DOWN 1   //+y
#define LEFT 2   //-x
#define RIGHT 3  //+x
#define OUTFILE "MAZE"
#define WHITE fprintf(outfile, "%c%c%c", 255,255,255)
#define BLACK fprintf(outfile, "%c%c%c", 0,0,0)
#define RED   fprintf(outfile, "%c%c%c", 0,0,255)

#define nodeadend//generate a maze without any dead ends! (consequently, many solutions to maze)
#define prim    //enable this to generate mazes using prim's algorithm.
//#define backtrack//enable this to generate mazes using depth-first search. Don't enable both.
#define movie   //this option spams bitmaps to illustrate each step of generation.

long numin=1;     //Number of cells in the maze.
const int xsize=20;
const int ysize=20;

int counter = 0;

void initialize();
void generate();
void savebmp(int xspecial, int yspecial);

struct cell{
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

cell MAZE[xsize][ysize];
