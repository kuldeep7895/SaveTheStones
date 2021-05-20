
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

clock_t globalClock;
//Render maze
void renderMaze();

int numAveng = 2;
			bool quit = false;
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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
				
				//Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
	
	for(int i = 0; i < numAveng; i++)
	{
	
		if( !gAvengersTexture[i].loadFromFile( "dot.bmp" ) )
		{
				
			printf( "Failed to load avenger %d texture!\n", i);
			success = false;
			
		}
	
	}
	
	//Load music
	gMusic = Mix_LoadMUS( "sounds/beat.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	//Load sound effects
	gScratch = Mix_LoadWAV( "sounds/scratch.wav" );
	if( gScratch == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	gHigh = Mix_LoadWAV( "sounds/high.wav" );
	if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gMedium = Mix_LoadWAV( "sounds/medium.wav" );
	if( gMedium == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gLow = Mix_LoadWAV( "sounds/low.wav" );
	if( gLow == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
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
	gStrengthTexture.free();

/*	for(int i = 0; i < 2; i++)
	{
		
		gAvengersTexture[i].free();
	
	}*/
	
	for(int i = 0; i < 6; i++)
	{
		
		gStoneTexture[i].free();
	
	}
	//gStoneTexture.free();
	
	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;
	
	//Free the sound effects
	Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( gMedium );
	Mix_FreeChunk( gLow );
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;
	
	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;


	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
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

string mazeToString(){
	string s = "";
	for (int y=0; y<GRID_HEIGHT; ++y)
	{
		 
		for (int x=0; x<GRID_WIDTH; ++x)
		{
			s+= grid[XYToIndex(x,y)].info;
		
		}
		s+=",";
		
	}
	return s;
}

void stringToMaze(string s){



//	cout<<" String  we received is ------------------------------------------------------"<<endl;
	//				cout<<s<<endl;
					
					
					
	int p = 0;
	int q = 0;

	for (int i = 0; i < s.length(); i++) {
		if(s[i]==','){
			q++;
			p = 0;
		}else{
		
			grid[XYToIndex(p,q)].info = s[i]; 
			p++;	
		}
    }
    
 //   cout<<"--------------------------------- My Data is --------------------"<<endl;
    /*for (int y=0; y<GRID_HEIGHT; ++y)
	{
		 
		for (int x=0; x<GRID_WIDTH; ++x)
		{
			cout<<grid[XYToIndex(x,y)].info;
		
		}
		
		cout<<endl;
			
	}*/
}

/* sone extra funcs */



  //static ChatScreen chatScreen; // Create static chatscreen object
  static int CLIENT_ID = -1;
  
  
  
  
// The Client's data will be managed by this class
  class ClientData
  {
  private:
  	int m_id;
  	std::string m_username;

  public:
  	ClientData(int id) : m_id(id) {}

  	void SetUsername(std::string username){ m_username = username; }

  	int GetID(){ return m_id; }
  	std::string GetUsername(){ return m_username; }
  };
  
  
  // A std::map of all clients that are currently connected.
  map<int,ClientData*> client_map; // id, ClientData*
  
  string dataReceived = "";
  

  void ParseData(char* data)
  {
  //	cout<<"Raw data "<<data<<endl;
  	// Will store the data type (e.g. 1, 2, etc)
  	int data_type;

  	// Will store the id of the client that is sending the data
  	int id;

  	// Get first two numbers from the data (data_type and id) and but them in their respective variables
  	sscanf(data,"%d|%d", &data_type, &id);
  	
  	if(id==CLIENT_ID){
  		clock_t time_elapsed;
  		time_elapsed = clock()- globalClock;
  		
  		
  		
  		if(((float)time_elapsed/CLOCKS_PER_SEC)>=0.01){
  			cout<<"*****************************Time Taken is ---------------------"<<(float)time_elapsed/CLOCKS_PER_SEC<<endl;
  			cout<<"*****************************Slow connection ---------------------"<<endl;
  			
  				
  		}
  	}

  	// Switch between the different data_types
  	switch(data_type)
  	{
      case 1: // data is a message
  			if(id != CLIENT_ID)
  			{
  				// Get message and Post it using the ClientData at id's username and the parsed msg.
  				char msg[8000000];
  				sscanf(data, "%*d|%*d|%[^|]", &msg);
  				//chatScreen.PostMessage(client_map[id]->GetUsername().c_str(), msg);
  			//	cout<<client_map[id]->GetUsername().c_str()<<": "<<msg<<endl;
  				dataReceived = msg;
  			}
  			break;
  		case 2: // data is a username
  			if(id != CLIENT_ID)
  			{
  				// Create a new ClientData with username and add it to map at id.
  				char username[80];
  				sscanf(data, "%*d|%*d|%[^|]", &username);

  				client_map[id] = new ClientData(id);
  				client_map[id]->SetUsername(username);
  			}
  			break;
  		case 3: // data is our ID.
  			CLIENT_ID = id; // Set our id to the received id.
  			break;
  	}
  }
  
  // A simple way to send packets to a peer
  void SendPacket(ENetPeer* peer, const char* data)
  {
    // Create the packet using enet_packet_create and the data we want to send
    // We are using the flag ENET_PACKET_FLAG_RELIABLE that acts a bit like TCP.
    // That is to say, it'll make sure the packet makes it to the destination.
  
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);

    // Finally, send the packet to the peer on channel 0!
    enet_peer_send(peer, 0, packet);
  }
    void* MsgLoop(ENetHost* client)
  {
  	while(!quit)
  	{
  		ENetEvent event;
  		while(enet_host_service(client, &event, 0) > 0)
  		{
  			switch(event.type)
  			{
  				case ENET_EVENT_TYPE_RECEIVE:

  					ParseData(event.packet->data); // Parse the receiving data.
  					enet_packet_destroy(event.packet);

  					break;
  			}
  		}
  	}
  }
   
   
   

int main( int argc, char* args[] )
{
	
	// Starting point and top-level control.
	srand( time(0) ); // seed random number generator.
	ResetGrid();
	bool posAssigned = false;

	
	printf("Please Enter Your Username?\n");
  	char username[80];
  	scanf("%s", &username);
  	
  		Visit(1,1);
  		createMaze();
		whiteSquares();
  
	
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


			//Event handler
			SDL_Event e;
			
			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };
			
			//The application timer
			LTimer timer;

			//In memory text stream
			std::stringstream timeText;
			
			//The thanos that will be moving around on the screen
			Thanos thanos;
			thanos.mPosX = grid[XYToIndex(1,0)].box.x;
			thanos.mPosY = grid[XYToIndex(1,0)].box.y;
			
			
			Dot avengers[numAveng];
			thanos.type = THANOS;
			thanos.strength = 120;
			
			avengers[0].strength = 60;
			avengers[1].strength = 70;
			
					for(int i = 0; i < numAveng; i++)
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
			if(string(username)=="thanos"){
				control = 0;
			}else{
				control = 1;
			}
			
			
			//bool realityActivated = false;
			
			float time_cur;
			
			int mix_count = 0;
			
			
			/* Game internet connection */
			
			// User set username
  

    // Normal enet initialization (see part 1)
  	if(enet_initialize() != 0)
  	{
  		fprintf(stderr, "An error occurred while initializing ENet!\n");
  		return EXIT_FAILURE;
  	}
  	atexit(enet_deinitialize);

  	ENetHost* client;
  	client = enet_host_create(NULL, 1, 1, 0, 0);

  	if(client == NULL)
  	{
  		fprintf(stderr, "An error occurred while trying to create an ENet client host!\n");
  		return EXIT_FAILURE;
  	}

  	ENetAddress address;
  	ENetEvent event;
  	ENetPeer* peer;

  	enet_address_set_host(&address, "127.0.0.1");
  	address.port = 7777;

  	peer = enet_host_connect(client, &address, 1, 0);
  	if(peer == NULL)
  	{
  		fprintf(stderr, "No available peers for initiating an ENet connection!\n");
  		return EXIT_FAILURE;
  	}

  	if(enet_host_service(client, &event, 5000) > 0 &&
  		event.type == ENET_EVENT_TYPE_CONNECT)
  	{
  		puts("Connection to 127.0.0.1:7777 succeeded.");
  	}
  	else
  	{
  		enet_peer_reset(peer);
  		puts("Connection to 127.0.0.1:7777 failed.");
  		return EXIT_SUCCESS;
  	}

  	// Send The Server The User's Username
  	char str_data[80] = "2|";
  	strcat(str_data, username);
  	SendPacket(peer, str_data);

    // Init the chatScreen
  	//chatScreen.Init();
  	
  	// Create a thread for receiving data
  	pthread_t thread;
  	pthread_create(&thread, NULL, MsgLoop, client);


			
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
							
							else
							{
							
								switch( e.key.keysym.sym )
								{
									//Play high sound effect
									case SDLK_1:
									Mix_PlayChannel( -1, gHigh, 0 );
									break;
									
									//Play medium sound effect
									case SDLK_2:
									Mix_PlayChannel( -1, gMedium, 0 );
									break;
									
									//Play low sound effect
									case SDLK_3:
									Mix_PlayChannel( -1, gLow, 0 );
									break;
									
									//Play scratch sound effect
									case SDLK_4:
									Mix_PlayChannel( -1, gScratch, 0 );
									break;
									
									case SDLK_9:
									//If there is no music playing
									if( Mix_PlayingMusic() == 0 )
									{
										//Play the music
										Mix_PlayMusic( gMusic, -1 );
									}
									//If music is being played
									else
									{
										//If the music is paused
										if( Mix_PausedMusic() == 1 )
										{
											//Resume the music
											Mix_ResumeMusic();
										}
										//If the music is playing
										else
										{
											//Pause the music
											Mix_PauseMusic();
										}
									}
									break;
									
									case SDLK_0:
									//Stop the music
									Mix_HaltMusic();
									break;
								}
							
							}
							
						}
						
						
						if(control == 0){
						
							thanos.handleEvent( e );
						
						}else{
							avengers[control-1].handleEvent(e);
						}
					
					
					}
						
				}
				
				float time_current =  timer.getTicks() / 1000.f;
				
				if(control==0){
					thanos.time = time_current - xtraTime;
				}

				//Move the thanos and avengers and check collision
				
				thanos.move(&(avengers[0]), &(avengers[1]));
				
				if(!realityActivated)
				{

					for(int i = 0; i < numAveng; i++)
					{
					
						avengers[i].move(&thanos);
					
					}
					
				}
				
				else
				{
				
					if(rPressed)
					{
					
						time_cur = time_current;
						rPressed = false;
						realityActivated = true;
					
					}
					
					else
					{
					
						if(time_current - time_cur > 5)
						{
						
							realityActivated = false;
						
						}
					
					}
				
				}
				
				for(int i = 0; i < 6; i++)
				{
				
					if(stone[i].check(thanos))
					{
					
						thanos.num_stones++;
						
						if(control == 0 && !contains(stone_type, stone[i].type))
						{
						
							Mix_PlayChannel( -1, gHigh, 0 );
							
						}
						stone_type.push_back(stone[i].type);	
						
					}
				
				}
				
				thanos.stones = stone_type;
				
				if( e.type == SDL_KEYDOWN )
				{
					
					if( (e.key.keysym.sym == SDLK_TAB) &&(control!=0) &&(e.key.repeat == 0) )
					{
						if(control==numAveng){
							control = 1;
						}else{
							control+=1;
						}
						
									
					}

					
					
					
								
				}
				
				thanos.control = control;
				
		
				//Set text to be rendered
				timeText.str( "" );
				timeText << "Strengths: Thanos - " << thanos.strength << ", Avenger 1 - " << avengers[0].strength << ", Avenger 2 - " << avengers[1].strength; 

				//Render text
				if( !gStrengthTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					printf( "Unable to render strength texture!\n" );
				}
				
				
				//cout << "Thanos\t" << thanos.strength << "\t" << thanos.power_factor << endl;
				//cout << "Avenger1\t" << avengers[0].strength << "\t" << avengers[0].power_factor << endl;
				//cout << "Avenger2\t" << avengers[1].strength << "\t" << avengers[1].power_factor << endl;
				//cout << thanos.xPressed << endl;
				//cout << soulPressed << endl;
				
				//Set text to be rendered
				timeText.str( "" );
				timeText << "Type: ";
				
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
				
				if(((thanos.time > 100) && (thanos.num_stones < 6))||(thanos.strength<0))
				{
				
					//Set text to be rendered
					timeText.str( "" );
					timeText << "Avengers Won!" ; 
					timer.stop();
					status = false;
					
					if(control == 0 && mix_count == 0)
					{
					
						Mix_PlayChannel( -1, gHigh, 0 );
						mix_count++;
						
					}

					//Render text
					if( !gLosePromptTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
					{
						printf( "Unable to render lose texture!\n" );
					}
				
				}
				
				//Set text to be rendered
				timeText.str( "" );
				timeText << "Time: " << (thanos.time) ; 

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
				
				bool avenDie = true;
				for(int i = 0;i<numAveng;i++){
					if(avengers[i].strength>0){
						avenDie = false; 
					}
				}
				
				if(((thanos.num_stones == 6) && (thanos.time <= 100))||avenDie)
				{
				
					//Set text to be rendered
					timeText.str( "" );
					timeText << "Thanos Won!" ; 
					timer.stop();
					status = false;
					
					if(control == 0 && mix_count == 0)
					{
					
						Mix_PlayChannel( -1, gHigh, 0 );
						mix_count++;
						
					}

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
				string msg;
				//cout<<username<<endl;
				if(string(username)=="thanos"){
					msg = mazeToString();
					
					msg+="*("+to_string(thanos.mPosX)+","+to_string(thanos.mPosY)+","+to_string(thanos.time)+","+to_string(xPressed)+","+to_string(mPressed)+","+to_string(soulPressed)+","+to_string(rPressed)+","+to_string(realityActivated)+")";
					
					for(int i=0;i<6;i++){
					
						msg+="*("+to_string(stone[i].type)+","+to_string(stone[i].pick)+","+to_string(stone[i].mPosX)+","+to_string(stone[i].mPosY)+")";
					
					}
					for(int i = 0;i<numAveng;i++){
					
						msg+="*("+to_string(avengers[i].type)+","+to_string(avengers[i].strength)+","+to_string(avengers[i].kill)+")";
					}
					msg+="*("+to_string(thanos.num_stones)+",";
					for(int p= 0;p<thanos.stones.size();p++){
						if(p==thanos.stones.size()-1){
							msg+=to_string(stone_type[p])+")";
						}else{
							msg+=to_string(stone_type[p])+",";
						}
					}
					
						char message_data[800000] = "1|";
				  		strcat(message_data, msg.c_str());
				  		SendPacket(peer, message_data);

				  		globalClock = clock();
				  		
				  		int strIndex = 2;
					//	cout<<dataReceived.length()<<endl;
						string dataR = dataReceived;
				  		int n = dataR.length();
						
//						
//						msg+="*("+to_string(avengers[i].type)+","+to_string(avengers[i].mPosX)+","+to_string(avengers[i].mPosY)+","+to_string(avengers[i].pPressed)+","+to_string(avengers[i].pPressedInactive)+")";
						for(int skip=0;skip<numAveng;skip++){
						
							if(skip!=0){
								strIndex+= 3;
							}
							
							int count = 0;
							while(strIndex<n){
								string num = "";
								while(strIndex<n){
									if(dataR[strIndex]==')'||dataR[strIndex]==','){
										break;
									}
									num+=dataR[strIndex];
									strIndex++;
								}
								
								
								int newNum;
							if(num==""){
							
								newNum = 0;
							
							}else{
							newNum = stoi(num);
							}
								if(count==0){
									avengers[skip].type = newNum;
								}else if(count==1){
									avengers[skip].mPosX = newNum;
								}else if (count==2){
									avengers[skip].mPosY = newNum;
								}else if (count==3){
									avengers[skip].pPressed = newNum;
								}else if (count==4){
									avengers[skip].pPressedInactive = newNum;
								}
								
								if(dataR[strIndex]==')'){
									break;
								}
								
								
								strIndex++;
								count++;
							
							}
					}
					
					
					
					strIndex+= 3;

					string num = "";
					while(strIndex<n){
						if(dataR[strIndex]==')'||dataR[strIndex]==','){
							break;
						}
						num+=dataR[strIndex];
						strIndex++;
					}
					//cout<<" Nm is "<<num<<endl;
					if(num==""){
						thanos.strength = 0;
					}else{
						thanos.strength = stoi(num);
					}
					
				//	cout<<"----------------------P Press -----------------------------"<<endl;
					for(int skip=0;skip<numAveng;skip++){
						if(avengers[skip].pPressed){
						//	cout<<"A WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"<<skip<<"  "<<avengers[skip].pPressed<<endl;
						}
					
					}
				
				}
				else{
				
				
				
					
					string strMaze = "";
					int strIndex = 0;
				//	cout<<dataReceived.length()<<endl;
					string dataR = dataReceived;
				//	cout<<" Data we received is ------------------------------------------------------"<<dataR<<endl;
					
										
					    int n = dataR.length();
					 
					    // declaring character array
					    char char_array[n + 1];
					 
					    // copying the contents of the
					    // string to char array
					    strcpy(char_array, dataR.c_str());
					while(strIndex<n){
						if(dataR[strIndex]=='*'){
							break;
						}
						strMaze+=dataR[strIndex];
						strIndex++;
					}
					
					
					
					
					strIndex+= 2;
					int count = 0;
					while(strIndex<n){
						string num = "";
						while(strIndex<n){
							if(dataR[strIndex]==')'||dataR[strIndex]==','){
								break;
							}
							num+=dataR[strIndex];
							strIndex++;
						}
						
						
						
						int newNum;
							if(num==""){
							
								newNum = 0;
							
							}else{
							newNum = stoi(num);
							}
						if(count==0){
							thanos.mPosX = newNum;
						}else if(count==1){
							thanos.mPosY = newNum;
						}else if(count==2){
							thanos.time = stof(num);
						}else if (count==3){
							xPressed = newNum;
						}else if (count==4){
							mPressed = newNum;
						}else if (count==5){
							soulPressed = newNum;
						}else if (count==6){
							rPressed = newNum;
						}else if (count==7){
							realityActivated = newNum;
						}
						if(dataR[strIndex]==')'){
							break;
						}
						strIndex++;
						count++;
					
					}
					
					for(int skip=0;skip<6;skip++){
						
						strIndex+= 3;
						int count = 0;
						while(strIndex<n){
							string num = "";
							while(strIndex<n){
								if(dataR[strIndex]==')'||dataR[strIndex]==','){
									break;
								}
								num+=dataR[strIndex];
								strIndex++;
							}
							
							int newNum;
							if(num==""){
							
								newNum = 0;
							
							}else{
							newNum = stoi(num);
							}
							if(count==0){
								stone[skip].type = newNum;
							}else if(count==1){
								stone[skip].pick = newNum;
							}else if (count==2){
								stone[skip].mPosX = newNum;
							}else{
								stone[skip].mPosY = newNum;
							}
							
							if(dataR[strIndex]==')'){
								break;
							}
							
							
							strIndex++;
							count++;
						
						}
						
						
					
					}

					

					for(int skip=0;skip<numAveng;skip++){
						
						strIndex+= 3;
						int count = 0;
						while(strIndex<n){
							string num = "";
							while(strIndex<n){
								if(dataR[strIndex]==')'||dataR[strIndex]==','){
									break;
								}
								num+=dataR[strIndex];
								strIndex++;
							}
							
							int newNum;
							if(num==""){
							
								newNum = 0;
							
							}else{
							newNum = stoi(num);
							}
							if(count==0){
								avengers[skip].type = newNum;
							}else if(count==1){
								avengers[skip].strength = newNum;
							}else if (count==2){
								avengers[skip].kill = newNum;
							}
							
							if(dataR[strIndex]==')'){
								break;
							}
							
							
							strIndex++;
							count++;
						
						}
					}
				


					strIndex+= 3;

					string num = "";
					while(strIndex<n){
						if(dataR[strIndex]==')'||dataR[strIndex]==','){
							break;
						}
						num+=dataR[strIndex];
						strIndex++;
					}
					//cout<<" Nm is "<<num<<endl;
					if(num==""){
						thanos.num_stones = 0;
					}else{
						thanos.num_stones = stoi(num);
					}
//					
					
					vector<int> localStones;
					
					if(dataR[strIndex]==','){
						strIndex+=1;
					}else if(dataR[strIndex]==')'){
						strIndex+= 3;
					}

						while(strIndex<n){
							string num = "";
							while(strIndex<n){
								if(dataR[strIndex]==')'||dataR[strIndex]==','){
									break;
								}
								num+=dataR[strIndex];
								strIndex++;
							}
							int newNum;
							if(num==""){
							
								newNum = 0;
							
							}else{
							newNum = stoi(num);
							}
							localStones.push_back(newNum);
														
							strIndex++;
					
						
						}
					stone_type = localStones;
				//	cout<<" ------------------------------New Print ----------------"<<endl;
					for(int myBad = 0 ;myBad<localStones.size();myBad++){
					//	cout<<localStones[myBad]<<"    ";
					}
				//	cout<<endl;
										
					stringToMaze(strMaze);
					createMaze();
					renderMaze();
					
					
				
					for(int i = 0;i<numAveng;i++){
					
						msg+="*("+to_string(avengers[i].type)+","+to_string(avengers[i].mPosX)+","+to_string(avengers[i].mPosY)+","+to_string(avengers[i].pPressed)+","+to_string(avengers[i].pPressedInactive)+")";
					
					}
					
					msg+="*("+to_string(thanos.strength)+")"; 
					
					char message_data[800000] = "1|";
					globalClock = clock();
				  		strcat(message_data, msg.c_str());
				  		SendPacket(peer, message_data);

				
				}
				
				
				
				
			
				
				
					
					for(int i = 0; i < 6; i++)
					{
					
						stone[i].render(i);
						//cout << "1. " << thanos.num_stones << " stones picked!" << endl;
					
					}
					
					
				//Render thanos
				thanos.render(&gThanosTexture);
				
				//Render avengers
				for(int i = 0; i < numAveng; i++)
				{
				
					avengers[i].render(&gAvengersTexture[i]);
				
				}
				
				//Render textures
				gStrengthTexture.render( ( SCREEN_WIDTH - gStartPromptTexture.getWidth() ) / 10, 0 );
				
				gTimeTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) - 50, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gStoneCountTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 10, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gWinPromptTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gLosePromptTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 8 );
				gStoneTypeTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 10, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 15 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
			
			
			  		// Join thread
		  	pthread_join(thread, NULL);

		    // Disconnect peer from server
		  	enet_peer_disconnect(peer, 0);

		  	while(enet_host_service(client, &event, 3000) > 0)
		  	{
		  		switch(event.type)
		  		{
		  			case ENET_EVENT_TYPE_RECEIVE:
		  				enet_packet_destroy(event.packet);
		  				break;
		  			case ENET_EVENT_TYPE_DISCONNECT:
		  				puts("Disconnection succeeded.");
		  				break;
		  		}
		  	}

		  	return EXIT_SUCCESS;
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
