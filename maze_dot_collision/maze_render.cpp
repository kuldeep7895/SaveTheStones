/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include "game.cpp"

Cell grid[GRID_WIDTH*GRID_HEIGHT];

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//Render maze
void renderMaze();

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

	//Load press texture
	if( !gDotTexture.loadFromFile( "dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	
	//Free loaded images
	gDotTexture.free();

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

void renderMaze()
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
		 	
		 	if(grid[XYToIndex(x,y)].info == '#')
		 	{
		 	
		 		//Render black filled quad
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );		
				SDL_RenderFillRect( gRenderer, &grid[XYToIndex(x,y)].box );
		 	
		 	}
		 	
		 	else
		 	{
		 	
		 		//Render black filled quad
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
				SDL_RenderFillRect( gRenderer, &grid[XYToIndex(x,y)].box );
		 	
		 	}
		 	
		 	//cout << x << " " << y << " " << grid[XYToIndex(x,y)].x_coor << " " << grid[XYToIndex(x,y)].y_coor << " " << grid[XYToIndex(x,y)].width << " " << grid[XYToIndex(x,y)].height << " " << grid[XYToIndex(x,y)].color << endl;
		 
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
			
			//The dot that will be moving around on the screen
			Dot dot;
			
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
					
					//Handle input for the dot
					dot.handleEvent( e );
					
				}

				//Move the dot and check collision
				dot.move();

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				renderMaze();
				
				//Render dot
				dot.render();

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
