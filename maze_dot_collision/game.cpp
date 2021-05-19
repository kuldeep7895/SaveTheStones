#include "game.h"

//Box collision detector
bool checkCollision( SDL_Rect a);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

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
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


//......................................................................................


LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

void LTimer::decrease()
{
	
	mStartTicks += 5000;	
	
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}

bool LTimer::check()
{
	
    return ((SDL_GetTicks() - mStartTicks) );
}


//.....................................................................................

vector<int> getPos()
{
	
	//srand(time(0));
	
	vector<int> a;	
		
	int x = rand();
	x = rand() % GRID_WIDTH;
	int y = rand();
	y = rand() % GRID_HEIGHT;
	
	//cout << "Fun1" << "\t" << x << "\t\t" << y << endl;
	
	while((grid[XYToIndex(x, y)].info == '#') || (grid[XYToIndex(x, y)].info == '.'))
	{
			
		x = rand();
		x = rand() % GRID_WIDTH;
		y = rand();
		y = rand() % GRID_HEIGHT;	
		
	//	cout << "Fun2" << "\t" << x << "\t\t" << y << endl;	
			
	}
	
	grid[XYToIndex(x, y)].info = '.';
	
	//cout << "Fun3" << "\t" << x << "\t\t" << y << endl;
	
	a.push_back(grid[XYToIndex(x, y)].box.x);
	a.push_back(grid[XYToIndex(x, y)].box.y);
	
//	cout << "Fun4" << "\t" << a[0] << "\t\t" << a[1] << endl;
	
	return a;

}

void whiteSquares()
{
	
	//srand(time(0));
	
	//vector<int> a;
	for(int i = 0; i < 20; i++)
	{	
		
		int x = rand();
		x = rand() % GRID_WIDTH;
		int y = rand();
		y = rand() % GRID_HEIGHT;
		
		//cout << "Fun1" << "\t" << x << "\t\t" << y << endl;
		
		while((grid[XYToIndex(x, y)].info == ' ') || (grid[XYToIndex(x, y)].info == '.') || (x == 0) || (x == GRID_WIDTH - 1) || (y == 0) || (y == GRID_WIDTH - 1))
		{
				
			x = rand();
			x = rand() % GRID_WIDTH;
			y = rand();
			y = rand() % GRID_HEIGHT;	
			
		//	cout << "Fun2" << "\t" << x << "\t\t" << y << endl;	
				
		}
		
		grid[XYToIndex(x, y)].info = ' ';
		
		//cout << "Fun3" << "\t" << x << "\t\t" << y << endl;
		
		grid[XYToIndex(x,y)].box.x = 70 + (SCREEN_WIDTH / 40)*x;
		grid[XYToIndex(x,y)].box.y = 130 + (SCREEN_WIDTH / 40)*y;
		grid[XYToIndex(x,y)].box.w = SCREEN_WIDTH / 40;
		grid[XYToIndex(x,y)].box.h = SCREEN_WIDTH / 40;
	
	}
		
//	cout << "Fun4" << "\t" << a[0] << "\t\t" << a[1] << endl;
	
	//return a;

}

bool checkCollision( SDL_Rect a )
{
    
	for (int y=0; y<GRID_HEIGHT; ++y)
	{
		 
		for (int x=0; x<GRID_WIDTH; ++x)
		{
			
			if(grid[XYToIndex(x, y)].info == '#')
			{
				
				//The sides of the rectangles
				int leftA, leftB;
				int rightA, rightB;
				int topA, topB;
				int bottomA, bottomB;

				//Calculate the sides of rect A
				leftA = a.x;
				rightA = a.x + a.w;
				topA = a.y;
				bottomA = a.y + a.h;

				//Calculate the sides of rect B
				leftB = grid[XYToIndex(x, y)].box.x;
				rightB = grid[XYToIndex(x, y)].box.x + grid[XYToIndex(x, y)].box.w;
				topB = grid[XYToIndex(x, y)].box.y;
				bottomB = grid[XYToIndex(x, y)].box.y + grid[XYToIndex(x, y)].box.h;
				
				bool flag = true;
				
				//If any of the sides from A are outside of B
				if( bottomA <= topB )
				{
				
					flag = false;
				
				}

				if( topA >= bottomB )
				{
			
					flag = false;
			
				}

				if( rightA <= leftB )
				{
				
					flag = false;
				
				}

				if( leftA >= rightB )
				{
				
					flag = false;
				
				}

				//If none of the sides from A are outside B
				if(flag)
					return true;
			
			}
			
		}
	
	}
	
	return false;
	
}
