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
		
		void decrease();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};


vector<bool> stones_used = {false, false, false, false, false, false};

//Scene textures
LTexture gThanosTexture;
LTexture gAvengersTexture[2];

LTexture gStoneTexture[6];

//Scene textures
LTexture gTimeTextTexture;
LTexture gStoneCountTextTexture;
LTexture gStoneTypeTexture;
LTexture gStrengthTexture;
LTexture gPausePromptTexture;
LTexture gStartPromptTexture;
LTexture gWinPromptTexture;
LTexture gLosePromptTexture;


vector<int> getPos();
bool checkCollision( SDL_Rect a );

float xtraTime = 0;

bool mPressed = false;
bool soulPressed = false;
bool rPressed = false;
bool realityActivated = false;
bool xPressed = false;

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;


class Thanos;


//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 2 ;
		
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
		
		bool kill;
		
		bool pPressedInactive;
		
		bool mazeCollision;
		
		clock_t startCollide;

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
		    kill = false;
		    
		    mazeCollision = true;

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
			float temp = 0;
			
			cout << "Type\t" << (*d).type << "\t" << type << endl;
	
			if(pPressed)
			{
				temp = 10;
				if(xPressed&&((*d).type==1)){
					temp = temp*2;
				}
				
				pPressedInactive = true;
			
			}
			
			//float diff = temp - (*d).power_factor;
			
			if(!mPressed&&!soulPressed)					
			{
			
				(*d).strength = (*d).strength - temp;
				cout << "If\t" << mPressed << endl;	
				
			}
			
			else if(pPressed&&mPressed)
			{	
			
				
				cout << "Else\t" << mPressed << endl;
				mPressed = false;
			
			}
			
			if(soulPressed&&pPressed){
				
				if(type == THANOS)
					xtraTime -= 20;		
					
				(*d).kill = true;
				(*d).strength = 0;
				soulPressed = false;
			}else if(!mPressed&&pPressed){
				(*d).strength = (*d).strength - temp;
				
			}
			
			
			if(strength <= 0)
			{
			
				kill = true;
			
			}
			
			if((*d).strength <= 0)
			{
			
				(*d).kill = true;
			
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
		    
		    int collision1;
		    int collision2;		    

		    //If the dot collided or went too far to the left or right
		    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision(mCollider) || (collision1 = check(*d)))
		    {
			//Move back
			mPosX -= mVelX;
			mCollider.x = mPosX;
			
			if(!pPressedInactive)
			{
			
				if(collision1)
				{
				
					recomputeStrength(d);
					startCollide = clock();
					
				}
				
			}
			
			else
			{
			
				if(((float)((clock() - startCollide) / CLOCKS_PER_SEC) > 3))
				{
				
					pPressedInactive = false;
					startCollide = clock();
				
				}
			
			}
				
		    }

		    //Move the dot up or down
		    mPosY += mVelY;
		    mCollider.y = mPosY;

		    //If the dot collided or went too far up or down
		    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider) || (collision2 = check(*d)))
		    {
			//Move back
			mPosY -= mVelY;
			mCollider.y = mPosY;
			
			if(!pPressedInactive)
			{
			
				if(collision2)
				{
				
					recomputeStrength(d);
					startCollide = clock();
					
				}
				
			}
			
			else
			{
			
				if(((float)((clock() - startCollide) / CLOCKS_PER_SEC) > 3))
				{
				
					pPressedInactive = false;
					startCollide = clock();
				
				}
			
			}
		    }
		
		}
		
		//Moves the dot and checks collision
		void move(Dot* d1, Dot* d2)
		{
		
			//Move the dot left or right
		    mPosX += mVelX;
		    mCollider.x = mPosX;

		    //If the dot collided or went too far to the left or right
		    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision(mCollider) || (check(*d1)) || (check(*d2)))
		    {
			
			int collision1 = check(*d1);
		    int collision2 = check(*d2);
			
			//Move back
			mPosX -= mVelX;
			mCollider.x = mPosX;
			
			if(!pPressedInactive)
			{
			
				if(collision1)
				{
				
					recomputeStrength(d1);
					startCollide = clock();
					
				}
				
			}
			
			else
			{
			
				if(((float)((clock() - startCollide) / CLOCKS_PER_SEC) > 1))
				{
				
					pPressedInactive = false;
					startCollide = clock();
				
				}
			
			}
			
			if(!pPressedInactive)
			{
			
				if(collision2)
				{
				
					recomputeStrength(d2);
					startCollide = clock();
					
				}
				
			}
			
			else
			{
			
				if(((float)((clock() - startCollide) / CLOCKS_PER_SEC) > 1))
				{
				
					pPressedInactive = false;
					startCollide = clock();
				
				}
			
			}
		    }

		    //Move the dot up or down
		    mPosY += mVelY;
		    mCollider.y = mPosY;

		    //If the dot collided or went too far up or down
		    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider) || (check(*d1)) || (check(*d2)))
		    {
			
			int collision1 = check(*d1);
		    int collision2 = check(*d2);
			//Move back
			mPosY -= mVelY;
			mCollider.y = mPosY;
			
			if(!pPressedInactive)
			{
			
				if(collision1)
				{
				
					recomputeStrength(d1);
					startCollide = clock();
					
				}
				
			}
			
			else
			{
			
				if(((float)((clock() - startCollide) / CLOCKS_PER_SEC) > 1))
				{
				
					pPressedInactive = false;
					startCollide = clock();
				
				}
			
			}
			
			if(!pPressedInactive)
			{
			
				if(collision2)
				{
				
					recomputeStrength(d2);
					startCollide = clock();
					
				}
				
			}
			
			else
			{
			
				if(((float)((clock() - startCollide) / CLOCKS_PER_SEC) > 1))
				{
				
					pPressedInactive = false;
					startCollide = clock();
				
				}
			
			}
		    }
		
		}

		//Shows the dot on the screen
		void render(LTexture* lText)
		{
		
			//Show the dot
			if(!kill)
				(*lText).render( mPosX, mPosY );
			
			else
				(*lText).free();
		
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
			
			//cout << "Killed!" << d.type << "\t"<< d.kill << endl;
			
			if(d.kill)
			{

			
				return false;
			}
			
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

bool contains(vector<int> stones, int stone)
{

	for(int i = 0; i < stones.size(); i++)
	{
	
		if(stones[i] == stone)
			return true;
	
	}
	
	return false;

}

void time_stone(float* time)
{

	*time -= 5;

}

class Thanos : public Dot
{

	public:

		
		float time;
		
		bool tPressed = false;
		bool tPressedInactive = false;
		clock_t tStartCollide;
		
		
		bool xPressedInactive = false;
		clock_t xStartCollide;
		
		
		
		int xpower_factor = 0;
		
		int control;
		
		vector<int> stones;
		
		void recomputeStrengthThanos(Dot* d)
		{
		
			float temp = 0;
	
			if(pPressed)
			{
				
				cout << "Special\t" << soulPressed << endl;
				
				if(!soulPressed)
					(*d).strength = (*d).strength - 20;
				
				else
				{
				
					(*d).kill = true;
				
					soulPressed = false;
				
				}
				
				
				pPressedInactive = true;
			
			}
			
			
			
			//float diff = temp - (*d).power_factor;
			
			
			
			
			
			if(strength <= 0)
			{
			
				kill = true;
			
			}
			
			if((*d).strength <= 0)
			{
			
				(*d).kill = true;
			
			}
		
		}
		
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
			    case SDLK_t:
			    	
			    	/*//if(!tPressedInactive)
			    	if()
				{
				
					if((!tPressed) && (contains(stones, TIME_STONE)) && (control == 0))
				    	{
				    	
					    	tPressed = true; 
					    	cout << "T pressed!......................................................................\n";
					    	time_stone(&time);
					    	tPressedInactive = true;
					    	tStartCollide = clock();
				    	
				    	}
					
				}
				
				else
				{
				
					if(((float)((clock() - tStartCollide) / CLOCKS_PER_SEC) > 10))
					{
					
						tPressedInactive = false;
						tStartCollide = clock();
					
					}
				
				}*/
				
				cout << (stones_used[TIME_STONE]) << "\t" << contains(stones, TIME_STONE) << "\t" << control << endl;
				
				if((!stones_used[TIME_STONE]) && contains(stones, TIME_STONE) && (control == 0))
				{
					
				//	cout << "Initime" << time << endl;
					xtraTime = 10;
					stones_used[TIME_STONE] = true;
					
				//	cout << "Initime" << time << endl;
					
				}
			    	
			    	break;
			    
			    case SDLK_x:
			    	
			    	
			    	cout << "X pressed!......................................................................\n";
			    	
			    	if((!stones_used[POWER_STONE]) && contains(stones, POWER_STONE) && (control == 0))
				{
					xPressed = true; 
				//	cout << "Initime" << time << endl;
					xpower_factor = 1;
					//strength += 30;
					stones_used[POWER_STONE] = true;
					
				//	cout << "Initime" << time << endl;
					
				}
			    	
			    	break;
			    
			    case SDLK_r:
			    
			    	cout << "R pressed!......................................................................\n";
			    	
			    	if((!stones_used[REALITY_STONE]) && contains(stones, REALITY_STONE) && (control == 0))
				{
					rPressed = true;
					realityActivated = true; 
					
					stones_used[REALITY_STONE] = true;
					
				//	cout << "Initime" << time << endl;
					
				}
			    	
			    	break;
			    
			    case SDLK_m:
			    	cout << "M pressed!......................................................................\n";
			    	
			    	if((!stones_used[MIND_STONE]) && contains(stones, MIND_STONE) && (control == 0))
				{
					mPressed = true; 
				//	cout << "Initime" << time << endl;
					//xpower_factor = 1;
					cout << "M pressed\n";
					//mazeCollision = false;
					stones_used[MIND_STONE] = true;
					
				//	cout << "Initime" << time << endl;
					
				}
				
				break;
				
			    case SDLK_g:
			    	cout << "G pressed!......................................................................\n";
			    	
			    	if((!stones_used[SOUL_STONE]) && contains(stones, SOUL_STONE) && (control == 0))
				{
					soulPressed = true; 
				//	cout << "Initime" << time << endl;
					//xpower_factor = 1;
					//cout << "M pressed\n";
					//mazeCollision = false;
					stones_used[SOUL_STONE] = true;
					
				//	cout << "Initime" << time << endl;
					
				}
				
				break;
				
			    case SDLK_k:
			    	cout << "Sp pressed!......................................................................\n";
			    	
			    	if((!stones_used[SPACE_STONE]) && contains(stones, SPACE_STONE) && (control == 0))
				{
				//	cout << "Initime" << time << endl;
					//xpower_factor = 1;
					vector<int> pos = getPos();
					
					mPosX = pos[0];
					mPosY = pos[1];
					//mazeCollision = false;
					stones_used[SPACE_STONE] = true;
					
				//	cout << "Initime" << time << endl;
					
				}
			    	
			    	break;
			    		
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
