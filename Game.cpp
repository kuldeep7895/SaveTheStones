#include "Game.h"



Game::Game(){

}
Game::~Game(){

}



void Game::init (const char* title,int x,int y,int w,int h,bool fullscreen){
	int flags  = 0;
	if(fullscreen){
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if(SDL_Init(SDL_INIT_EVERYTHING)==0){
		
		cout<<"initialsise "<<endl;
		window  = SDL_CreateWindow(title,x,y,w,h,flags);
		
		if(window){
			cout<<"window created "<<endl;
			ScreenSurface = SDL_GetWindowSurface( window );
		}
		if(renderer){
			SDL_SetRenderDrawColor(renderer,255,255,255,255);
			cout<<"render created "<<endl;
		}
		isRunning  = true;
		loadImage = SDL_LoadBMP( "maze.bmp" );
	    	if( loadImage == NULL )
	    	{
			printf( "Unable to load image %s! SDL Error: %s\n", "maze.bmp", SDL_GetError() );
		
	    	}
	    	SDL_BlitSurface( loadImage, NULL, ScreenSurface, NULL );
		SDL_UpdateWindowSurface( window );
	}else{
		isRunning = false;
	}
	
}

void Game::update(){
	return;
}

bool Game::running(){
	return isRunning;
}

void Game::handleEvents(){
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type){
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
	
}

void Game::render(){
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Game::clean(){
	 SDL_DestroyWindow(window);
	 SDL_DestroyRenderer(renderer);
	 SDL_Quit();
	 cout<<"game clean"<<endl;
}

