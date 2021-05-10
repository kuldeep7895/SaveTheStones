#include "functions.h"

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The application time based timer
class LTimer
{
    public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};


//Scene textures
LTexture gDotTexture;
LTexture gStoneTexture[6];

//Scene textures
LTexture gTimeTextTexture;
LTexture gPausePromptTexture;
LTexture gStartPromptTexture;

vector<int> getPos();

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 2;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and checks collision
		void move();

		//Shows the dot on the screen
		void render();
		
		int num_stones;
		
		//Dot's collision box
		SDL_Rect mCollider;

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
		
};

class Stone
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		bool pick;

		//Initializes the variables
		Stone()
		{
	
			vector<int> pos = getPos();
			
			cout << "Constructor" << "\t" << pos[0] << "\t" << pos[1] << endl;
			
			//Initialize the offsets
		    	mPosX = pos[0];
		    	mPosY = pos[1];

			//Set collision box dimension
			mCollider.w = DOT_WIDTH;
			mCollider.h = DOT_HEIGHT;
			mCollider.x = mPosX;
			mCollider.y = mPosY;
			
			pick = false;

		}

		//Moves the dot and checks collision
		bool check(Dot d)
		{

			if(!pick)
			{

				//The sides of the rectangles
				int leftA, leftB;
				int rightA, rightB;
				int topA, topB;
				int bottomA, bottomB;

				//Calculate the sides of rect A
				leftA = d.mCollider.x;
				rightA = d.mCollider.x + d.mCollider.w;
				topA = d.mCollider.y;
				bottomA = d.mCollider.y + d.mCollider.h;

				//Calculate the sides of rect B
				leftB = mCollider.x;
				rightB = mCollider.x + mCollider.w;
				topB = mCollider.y;
				bottomB = mCollider.y + mCollider.h;
							
				//If any of the sides from A are outside of B
				if( bottomA <= topB )
				{
							
					return false;
					
				}

				if( topA >= bottomB )
				{
				
					return false;
						
				}

				if( rightA <= leftB )
				{
							
					return false;
							
				}

				if( leftA >= rightB )
				{
							
					return false;
							
				}

				//If none of the sides from A are outside B
				pick = true;
				return true;
			
			}

		}

		//Shows the dot on the screen
		
		void render(int i)
		{
		    //Show the stone
		    if(!pick)
			gStoneTexture[i].render( mPosX, mPosY );
		}

   
		//The X and Y offsets of the dot
		int mPosX, mPosY;
		

		
		//Dot's collision box
		SDL_Rect mCollider;
};

class Time_Stone : public Stone
{

	

};

class Power_Stone : public Stone
{

	

};

class Soul_Stone : public Stone
{

	

};

class Mind_Stone : public Stone
{

	

};

class Reality_Stone : public Stone
{

	

};

class Space_Stone : public Stone
{

	

};
