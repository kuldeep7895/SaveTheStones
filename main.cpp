#include "Game.cpp"#include "mazegen.cpp"Game *game  = nullptr;int main(){	game = new Game();	srand((unsigned int)time(NULL)); //seed random number generator with system time	initialize();      //initialize the maze	generate();        //generate the maze			#ifdef movie				for(int i=1;i<10;i++){			numin++;						savebmp(0,0);      //output the bitmap					}			#else movie		savebmp(0,0);	#endif	game->init("MyGame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,false);	while(game->running()){		game->handleEvents();		game->update();		game->render();	}	game->clean();	return 0;}