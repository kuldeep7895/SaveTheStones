#include "functions.h"

//Cell grid[GRID_WIDTH*GRID_HEIGHT];

// FUNCTIONS DEFINITIONS -----
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

void createMaze()
{

	 // Displays the finished maze to the screen.
	 
	 for (int y=0; y<GRID_HEIGHT; ++y)
	 {
		 
		 for (int x=0; x<GRID_WIDTH; ++x)
		 {
		 	if(x == 1 && y == 0)
		 		grid[XYToIndex(x,y)].info = ' ';
		 	//cout << grid[XYToIndex(x,y)];
		 	
		 	grid[XYToIndex(x,y)].box.x = 40 + (SCREEN_WIDTH / 40)*x;
			grid[XYToIndex(x,y)].box.y = 40 + (SCREEN_WIDTH / 40)*y;
			grid[XYToIndex(x,y)].box.w = SCREEN_WIDTH / 40;
			grid[XYToIndex(x,y)].box.h = SCREEN_WIDTH / 40;
		 	
		 	//cout << x << " " << y << " " << grid[XYToIndex(x,y)].x_coor << " " << grid[XYToIndex(x,y)].y_coor << " " << grid[XYToIndex(x,y)].width << " " << grid[XYToIndex(x,y)].height << " " << grid[XYToIndex(x,y)].color << endl;
		 
		 }
		 
		// cout << endl;
	 
	 }
	 
//	 cout << endl;

}
