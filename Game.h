#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

class Game{

public:
	Game();
	~Game();
	void init (const char* title,int x,int y,int w,int h,bool fullscreen);
	void update();
	void render();
	void clean();
	void handleEvents();
	
	bool running();
	
private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer; 
	//The surface contained by the window
	SDL_Surface* ScreenSurface = NULL;

	//The image we will load and show on the screen
	SDL_Surface* loadImage = NULL;


};
