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
				
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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
	
	//Open the font
	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };
		
		//Load stop prompt texture
		if( !gStartPromptTexture.loadFromRenderedText( "Press S to Start or Stop the Timer", textColor ) )
		{
			printf( "Unable to render start/stop prompt texture!\n" );
			success = false;
		}
		
		//Load pause prompt texture
		if( !gPausePromptTexture.loadFromRenderedText( "Press P to Pause or Unpause the Timer", textColor ) )
		{
			printf( "Unable to render pause/unpause prompt texture!\n" );
			success = false;
		}
	}

	//Load press texture
	if( !gDotTexture.loadFromFile( "dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
	
	for(int i = 0; i < 6; i++)
	{
		
		if( !gStoneTexture[i].loadFromFile( "stone.bmp" ) )
		{
			
			printf( "Failed to load stone %d texture!\n", i);
			success = false;
		
		}
	
	}

	return success;
}

void close()
{
	
	//Free loaded images
	gDotTexture.free();
	gTimeTextTexture.free();
	gStartPromptTexture.free();
	gPausePromptTexture.free();
	
	for(int i = 0; i < 6; i++)
	{
		
		gStoneTexture[i].free();
	
	}
	//gStoneTexture.free();
	
	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;


	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
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

	for (int y=0; y<GRID_HEIGHT; ++y)
	{
		 
		for (int x=0; x<GRID_WIDTH; ++x)
		{
	
			if(grid[XYToIndex(x,y)].info == '#')
			{
			 	
				//Render black filled quad
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );		
				SDL_RenderFillRect( gRenderer, &grid[XYToIndex(x,y)].box );
				 	
			}
				 	
			else
			{
				 	
				//Render white filled quad
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
				SDL_RenderFillRect( gRenderer, &grid[XYToIndex(x,y)].box );
				 	
			}
		
		}
		
	}

}

int main( int argc, char* args[] )
{
	
	// Starting point and top-level control.
	srand( time(0) ); // seed random number generator.
	ResetGrid();
	Visit(1,1);
	createMaze();
	
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
			
			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };
			
			//The application timer
			LTimer timer;

			//In memory text stream
			std::stringstream timeText;
			
			//The dot that will be moving around on the screen
			Dot dot;
			Stone stone[6];
			
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
					//Reset start time on return keypress
					else if( e.type == SDL_KEYDOWN )
					{
						//Start/stop
						if( e.key.keysym.sym == SDLK_s )
						{
							if( timer.isStarted() )
							{
								timer.stop();
							}
							else
							{
								timer.start();
							}
						}
						//Pause/unpause
						else if( e.key.keysym.sym == SDLK_p )
						{
							if( timer.isPaused() )
							{
								timer.unpause();
							}
							else
							{
								timer.pause();
							}
						}
					}
					
					//Handle input for the dot
					dot.handleEvent( e );
					
				}

				//Move the dot and check collision
				dot.move();
				
				for(int i = 0; i < 6; i++)
				{
				
					if(stone[i].check(dot))
						dot.num_stones++;
				
				}
				
				//Set text to be rendered
				timeText.str( "" );
				timeText << "Seconds since start time " << ( timer.getTicks() / 1000.f ) ; 

				//Render text
				if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					printf( "Unable to render time texture!\n" );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				renderMaze();
				
				for(int i = 0; i < 6; i++)
				{
				
					stone[i].render(i);
					cout << "1. " << dot.num_stones << " stones picked!" << endl;
				
				}
				
				//Render dot
				dot.render();
				
				//Render textures
				gStartPromptTexture.render( ( SCREEN_WIDTH - gStartPromptTexture.getWidth() ) / 2, 0 );
				gPausePromptTexture.render( ( SCREEN_WIDTH - gPausePromptTexture.getWidth() ) / 2, gStartPromptTexture.getHeight() );
				gTimeTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
