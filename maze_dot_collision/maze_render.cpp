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
	if( !gThanosTexture.loadFromFile( "thanos.bmp" ) )
	{
		printf( "Failed to load thanos texture!\n" );
		success = false;
	}
	
	if( !gStoneTexture[POWER_STONE].loadFromFile( "power_stone.bmp" ) )
	{
			
		printf( "Failed to load stone %d texture!\n", POWER_STONE);
		success = false;
		
	}
	
	if( !gStoneTexture[MIND_STONE].loadFromFile( "mind_stone.bmp" ) )
	{
			
		printf( "Failed to load stone %d texture!\n", MIND_STONE);
		success = false;
		
	}
	
	if( !gStoneTexture[REALITY_STONE].loadFromFile( "reality_stone.bmp" ) )
	{
			
		printf( "Failed to load stone %d texture!\n", REALITY_STONE);
		success = false;
		
	}
	
	if( !gStoneTexture[TIME_STONE].loadFromFile( "time_stone.bmp" ) )
	{
			
		printf( "Failed to load stone %d texture!\n", TIME_STONE);
		success = false;
		
	}
	
	if( !gStoneTexture[SPACE_STONE].loadFromFile( "space_stone.bmp" ) )
	{
			
		printf( "Failed to load stone %d texture!\n", SPACE_STONE);
		success = false;
		
	}
	
	if( !gStoneTexture[SOUL_STONE].loadFromFile( "soul_stone.bmp" ) )
	{
			
		printf( "Failed to load stone %d texture!\n", SOUL_STONE);
		success = false;
		
	}
	
	for(int i = 0; i < 2; i++)
	{
	
		if( !gAvengersTexture[i].loadFromFile( "dot.bmp" ) )
		{
				
			printf( "Failed to load avenger %d texture!\n", i);
			success = false;
			
		}
	
	}

	return success;
}

void close()
{
	
	//Free loaded images
	gThanosTexture.free();
	gTimeTextTexture.free();
	gStartPromptTexture.free();
	gStoneCountTextTexture.free();
	gStoneTypeTexture.free();
	gPausePromptTexture.free();
	gWinPromptTexture.free();
	gLosePromptTexture.free();
	
	for(int i = 0; i < 2; i++)
	{
		
		gAvengersTexture[i].free();
	
	}
	
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
			
			//The thanos that will be moving around on the screen
			Dot thanos, avengers[2];
			thanos.type = THANOS;
			thanos.strength = 120;
			avengers[0].strength = 60;
			avengers[1].strength = 70;
			
			for(int i = 0; i < 2; i++)
			{
			
				avengers[i].assignPos();
				
				avengers[i].type = AVENGER;
			
			}
			
			Stone stone[6];
			
			stone[POWER_STONE].type = POWER_STONE;
			stone[MIND_STONE].type = MIND_STONE;
			stone[SPACE_STONE].type = SPACE_STONE;
			stone[TIME_STONE].type = TIME_STONE;
			stone[REALITY_STONE].type = REALITY_STONE;
			stone[SOUL_STONE].type = SOUL_STONE;
			
			bool status = true;
			
			vector<int> stone_type;
			
			int control = 0;
			
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
					
					else if(status)
					{
					
						//Reset start time on return keypress
						if( e.type == SDL_KEYDOWN )
						{
							//Start/stop
							if( (e.key.keysym.sym == SDLK_UP) || (e.key.keysym.sym == SDLK_DOWN) || (e.key.keysym.sym == SDLK_LEFT) || (e.key.keysym.sym == SDLK_RIGHT) )
							{
								if( !timer.isStarted() )
								{
									timer.start();
								}
									
							}
							
						}
							
						switch(control)
						{
						
							case 0:
								thanos.handleEvent( e );
								break;
							
							case 1:
								avengers[0].handleEvent(e);
								break;
							
							case 2:
								avengers[1].handleEvent(e);
								break;
						
						}
					
					}
						
				}
				

				//Move the thanos and avengers and check collision
				thanos.move(&avengers[0], &avengers[1]);
				
				for(int i = 0; i < 2; i++)
				{
				
					avengers[i].move(&thanos);
				
				}
				
				for(int i = 0; i < 6; i++)
				{
				
					if(stone[i].check(thanos))
					{
					
						thanos.num_stones++;
						stone_type.push_back(stone[i].type);	
						
					}
				
				}
				
				if( e.type == SDL_KEYDOWN )
				{
					
					if( e.key.keysym.sym == SDLK_TAB )
					{
						control = (control + 1) % 3;
									
					}
								
				}
				
			/*	switch(control)
				{
				
					case 0:
						
						for(int i = 0; i < 2; i++)
						{
						
							if(thanos.check(avengers[i]))
							{
								cout << "1...........\n";
								recomputeStrength(&thanos, &avengers[i]);
//								thanos.pPressedInactive = true;
							
							}
							
						}
						
						break;
						
					case 1:
						
						if(avengers[0].check(thanos))
						{
							cout << "2...........\n";
							recomputeStrength(&avengers[0], &thanos);
							
						}
						
						break;
					
					case 2:
						
						if(avengers[1].check(thanos))
						{
							cout << "3...........\n";
							recomputeStrength(&avengers[1], &thanos);
							
						}
						
						break;
				
				}*/
				
				
				cout << "Thanos\t" << thanos.strength << "\t" << thanos.power_factor << endl;
				cout << "Avenger1\t" << avengers[0].strength << "\t" << avengers[0].power_factor << endl;
				cout << "Avenger2\t" << avengers[1].strength << "\t" << avengers[1].power_factor << endl;
				
				//Set text to be rendered
				timeText.str( "" );
				
				for (int i = 0; i < stone_type.size(); i++)
				{
				 
       				switch (stone_type[i])
       				{
       				
       					case POWER_STONE:
       						timeText << "PS ";
       						break; 
       					
       					case TIME_STONE:
       						timeText << "TS ";
       						break; 
       					
       					case MIND_STONE:
       						timeText << "MS ";
       						break; 
       						
       					case SPACE_STONE:
       						timeText << "SpS ";
       						break; 
       					
       					case REALITY_STONE:
       						timeText << "RS ";
       						break; 
       					
       					case SOUL_STONE:
       						timeText << "SoS ";
       						break; 
       				
       				}	
				
				}
				
				//Render text
				if( !gStoneTypeTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					//printf( "Unable to render stone type texture!\n" );
				}
				
				float time_current =  timer.getTicks() / 1000.f;
				
				if((time_current > 100) && (thanos.num_stones < 6))
				{
				
					//Set text to be rendered
					timeText.str( "" );
					timeText << "Lose!" ; 
					timer.stop();
					status = false;

					//Render text
					if( !gLosePromptTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
					{
						printf( "Unable to render lose texture!\n" );
					}
				
				}
				
				//Set text to be rendered
				timeText.str( "" );
				timeText << "Time: " << (time_current) ; 

				//Render text
				if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					printf( "Unable to render time texture!\n" );
				}
				
				//Set text to be rendered
				timeText.str( "" );
				timeText << "Stones: " << ( thanos.num_stones ) ; 

				//Render text
				if( !gStoneCountTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					printf( "Unable to render stone count texture!\n" );
				}
				
				if((thanos.num_stones == 6) && (time_current <= 100))
				{
				
					//Set text to be rendered
					timeText.str( "" );
					timeText << "Win!" ; 
					timer.stop();
					status = false;

					//Render text
					if( !gWinPromptTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
					{
						printf( "Unable to render win texture!\n" );
					}
				
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				renderMaze();
				
				for(int i = 0; i < 6; i++)
				{
				
					stone[i].render(i);
					//cout << "1. " << thanos.num_stones << " stones picked!" << endl;
				
				}
				
				//Render thanos
				thanos.render(&gThanosTexture);
				
				//Render avengers
				for(int i = 0; i < 2; i++)
				{
				
					avengers[i].render(&gAvengersTexture[i]);
				
				}
				
				//Render textures
				gStartPromptTexture.render( ( SCREEN_WIDTH - gStartPromptTexture.getWidth() ) / 2, 0 );
				gPausePromptTexture.render( ( SCREEN_WIDTH - gPausePromptTexture.getWidth() ) / 2, gStartPromptTexture.getHeight() );
				gTimeTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) - 50, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gStoneCountTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 10, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gWinPromptTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gLosePromptTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gStoneTypeTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 10, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 15 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

