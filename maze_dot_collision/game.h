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
		
		int init_time = 50;

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool check();
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
LTexture gThanosTexture;
LTexture gAvengersTexture[2];

LTexture gStoneTexture[6];

//Scene textures
LTexture gTimeTextTexture;
LTexture gStoneCountTextTexture;
LTexture gStoneTypeTexture;
LTexture gPausePromptTexture;
LTexture gStartPromptTexture;
LTexture gWinPromptTexture;
LTexture gLosePromptTexture;


vector<int> getPos();
bool checkCollision( SDL_Rect a );

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 2;
		
		//Dot's collision box
		SDL_Rect mCollider;

    
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
		
		int num_stones;
		
		float strength;
		
		int power_factor;
		
		int type;
		
		//int factor = 1;
		
		bool pPressed;
		
		bool pPressedInactive;

		//Initializes the variables
		Dot()
		{
		
			//Initialize the offsets
		    mPosX = 0;
		    mPosY = 0;
		    
		    //strength = 100;
		    
		    power_factor = 1;		//1.2    0.6
		    //factor = 1;
		    
		    pPressed = false;
		    pPressedInactive = false;

			//Set collision box dimension
			mCollider.w = DOT_WIDTH;
			mCollider.h = DOT_HEIGHT;

		    //Initialize the velocity
		    mVelX = 0;
		    mVelY = 0;
		    
		    num_stones = 0;
		
		}

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e )
		{
    
		    //If a key was pressed
		    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
		    {

			//Adjust the velocity
			switch( e.key.keysym.sym )
			{
			    case SDLK_UP: mVelY -= DOT_VEL; break;
			    case SDLK_DOWN: mVelY += DOT_VEL; break;
			    case SDLK_LEFT: mVelX -= DOT_VEL; break;
			    case SDLK_RIGHT: mVelX += DOT_VEL; break;
			    case SDLK_p: pPressed = true; cout << "P pressed!......................................................................\n";break;
			}
		    }
		    //If a key was released
		    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
		    {
			//Adjust the velocity
			switch( e.key.keysym.sym )
			{
			    case SDLK_UP: mVelY += DOT_VEL; break;
			    case SDLK_DOWN: mVelY -= DOT_VEL; break;
			    case SDLK_LEFT: mVelX += DOT_VEL; break;
			    case SDLK_RIGHT: mVelX -= DOT_VEL; break;
			    case SDLK_p: pPressed = false; cout << "P released!......................................................................\n";break;
			}
		    }
		
		}
		
		void recomputeStrength(Dot* d)
		{
		
			float temp = power_factor;
	
			if(pPressed)
				temp = power_factor + 1;
			
			float diff = temp - (*d).power_factor;
			
			if(diff > 0)
			{
								
				(*d).strength = (*d).strength - diff * 10;
										
			}
			
			else if(diff < 0)
			{
			
				strength = strength + diff * 10;
			
			}
		
		}

		//Moves the dot and checks collision
		void move()
		{
		
			//Move the dot left or right
		    mPosX += mVelX;
		    mCollider.x = mPosX;

		    //If the dot collided or went too far to the left or right
		    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision(mCollider) )
		    {
			//Move back
			mPosX -= mVelX;
			mCollider.x = mPosX;
		    }

		    //Move the dot up or down
		    mPosY += mVelY;
		    mCollider.y = mPosY;

		    //If the dot collided or went too far up or down
		    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider) )
		    {
			//Move back
			mPosY -= mVelY;
			mCollider.y = mPosY;
		    }
		
		}
		
		//Moves the dot and checks collision
		void move(Dot* d)
		{
		
			//Move the dot left or right
		    mPosX += mVelX;
		    mCollider.x = mPosX;
		    
		    int collision;

		    //If the dot collided or went too far to the left or right
		    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision(mCollider) || (collision = check(*d)))
		    {
			//Move back
			mPosX -= mVelX;
			mCollider.x = mPosX;
			
			if(collision)
			{
			
				recomputeStrength(d);
			
			}
		    }

		    //Move the dot up or down
		    mPosY += mVelY;
		    mCollider.y = mPosY;

		    //If the dot collided or went too far up or down
		    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider) || (collision = check(*d)))
		    {
			//Move back
			mPosY -= mVelY;
			mCollider.y = mPosY;
			
			if(collision)
			{
			
				recomputeStrength(d);
			
			}
		    }
		
		}
		
		//Moves the dot and checks collision
		void move(Dot* d1, Dot* d2)
		{
		
			//Move the dot left or right
		    mPosX += mVelX;
		    mCollider.x = mPosX;
		    
		    int collision1;
		    int collision2;

		    //If the dot collided or went too far to the left or right
		    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision(mCollider) || (collision1 = check(*d1)) || (collision2 = check(*d2)))
		    {
			//Move back
			mPosX -= mVelX;
			mCollider.x = mPosX;
			
			if(collision1)
			{
			
				recomputeStrength(d1);
			
			}
			
			if(collision2)
			{
			
				recomputeStrength(d2);
			
			}
		    }

		    //Move the dot up or down
		    mPosY += mVelY;
		    mCollider.y = mPosY;

		    //If the dot collided or went too far up or down
		    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider) || (collision1 = check(*d1)) || (collision2 = check(*d2)))
		    {
			//Move back
			mPosY -= mVelY;
			mCollider.y = mPosY;
			
			if(collision1)
			{
			
				recomputeStrength(d1);
			
			}
			
			if(collision2)
			{
			
				recomputeStrength(d2);
			
			}
		    }
		
		}

		//Shows the dot on the screen
		void render(LTexture* lText)
		{
		
			//Show the dot
			(*lText).render( mPosX, mPosY );
		
		}
		
		
		
		void assignPos()
		{
		
			vector<int> pos = getPos();
    
		    //grid[XYToIndex(pos[0], pos[1])].info = 'A';
		    
		    mPosX = pos[0];
		    mPosY = pos[1];
		
		}
		
		void power(int i)
		{
		
			power_factor += 0.5 * i ;
		
		}
		
		//Moves the dot and checks collision
		bool check(Dot d)
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
				//pick = true;
				return true;
			
			

		}
		
};

class Stone
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		bool pick;
		
		int type;

		//Initializes the variables
		Stone()
		{
	
			vector<int> pos = getPos();
			
			//cout << "Constructor" << "\t" << pos[0] << "\t" << pos[1] << endl;
			
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
