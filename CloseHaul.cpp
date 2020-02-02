//Using SDL and standard IO
#include "SDL2/SDL.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

//Texture wrapper class
class LTexture
{
	public:
		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
//		void render( int x, int y, SDL_Rect* clip = NULL );
        //Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        
        //Max Velocity
        static const int LTEX_VEL = 320;
        
        //Initializes variables
		LTexture();
        
        //Takes key presses and adjusts velocity
        void handleEvent( SDL_Event& e );
        
        //Moves the ship
        void move( float timeStep );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
        
        //Track the sprite's position and velocity
        float mPosX, mPosY;
        float mVelX, mVelY;
};

//The application time based timer
class LTimer
{//Start up SDL and create the window
    public:bool init();
		//Initializes variables
		LTimer();//Loads media
bool loadMedia();
		//The various clock actions
		void start();//Frees media and shuts down SDL2
		void stop();void close();
		void pause();
		void unpause();//The window to render to
SDL_Window *window;
		//Gets the timer's time
		Uint32 getTicks();//The surface contained by the window
SDL_Surface *surface;
		//Checks the status of the timer
		bool isStarted();//The SDL Renderer
		bool isPaused();SDL_Renderer *renderer;

    private://SDL Texture
		//The clock time when the timer startedSDL_Texture *texture;
		Uint32 mStartTicks;
//png Texture
		//The ticks stored when the timer was pausedSDL_Surface *pngSurface;
		Uint32 mPausedTicks;SDL_Texture *pngTexture;

		//The timer status//Ship Texture
		bool mPaused;SDL_Surface *shipSurface;
		bool mStarted;SDL_Texture *shipTexture;
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//pngSurface
SDL_Surface* pngSurface;
SDL_Texture* pngTexture;

//Scene sprites
SDL_Rect gSpriteClip;
LTexture shipLTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

void LTexture::move( float timeStep )
{
    //Move texture sprite left or right
    mPosX += mVelX * timeStep;
    
    //If the textured sprite goes too far:
    
    //Move textured sprite up or down
    mPosY += mVelY * timeStep;
    
    //If the textured sprite goes too far up/down:
    if ( mPosY < 0 )
    {
        mPosY = 0;
    }
    else if ( mPosY > SCREEN_HEIGHT - mHeight )
    {
        mPosY = SCREEN_HEIGHT - mHeight;
    }
}

LTexture::~LTexture()
{
	//Deallocate
	free();
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

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
        mPosX = 0;
        mPosY =0;
        mVelX = 0;
        mVelY = 0;
	}
}
void close()
{
    //Deallocate surface
//    SDL_FreeSurface(surface);
    SDL_FreeSurface(pngSurface);
//    SDL_FreeSurface(shipSurface);
    
    //Free loaded images
    shipLTexture.free();
    
    //Destroy Window
//    SDL_DestroyTexture(texture);
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
void LTexture::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= LTEX_VEL; break;
            //case SDLK_UP: printf("pressed up"); break;
            case SDLK_DOWN: mVelY += LTEX_VEL; break;
            case SDLK_LEFT: mVelX -= LTEX_VEL; break;
            case SDLK_RIGHT: mVelX += LTEX_VEL; break;
            default: printf("Pressed a Key\n"); break;
        }	
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += LTEX_VEL; break;
            case SDLK_DOWN: mVelY -= LTEX_VEL; break;
            case SDLK_LEFT: mVelX += LTEX_VEL; break;
            case SDLK_RIGHT: mVelX -= LTEX_VEL; break;
            case SDLK_ESCAPE: close(); break;
            default: printf("Released a key\n"); break;
        }
    }
}

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
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) )
		{
			printf( "Warning: Nearest texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Close Haul", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
    return success;
}

bool loadMedia()
{
    //Loading Success flag
    bool success = true;
    
    //Initialize PNG Loading
    int imgFlags = IMG_INIT_PNG;
    if( !( 2 & imgFlags ) )
    {
        printf( "SDL_image could not initialize SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }
    pngSurface = IMG_Load("Assets/CloseHaulBG.png");
    if ( pngSurface == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    pngTexture = SDL_CreateTextureFromSurface(gRenderer, pngSurface);
    if(!pngTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    SDL_FreeSurface(pngSurface);
    
    //Load sprite sheet texture
	if( !shipLTexture.loadFromFile( "Assets/Ship.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
		//Set ship sprite
		gSpriteClip.x =   0;
		gSpriteClip.y =   0;
		gSpriteClip.w = 108;
		gSpriteClip.h = 95;

    }
    return success;
}


int main(int argc, char *argv[])
{
    //Start up SDL and create window&renderer
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
        
            //Keeps track of the time between steps
            LTimer stepTimer;
            
            //Event handler
            SDL_Event e;
            
            //While application is running
            while ( !quit )
            {
                //Handle events on queue
                while ( SDL_PollEvent(&e) != 0 )
                {
                    //User requests quit
                    if (e.type == SDL_QUIT) 
                    {
                        quit = true;
                    }
                    //Handle input for the Ship
                    shipLTexture.handleEvent( e );
                    
//                    //User presses a key
//                    else if ( event.type == SDL_KEYDOWN )
//                        //Select surfaces based on key press
//                        switch( event.key.keysym.sym )
//                        {
//                            case SDLK_UP:
//                            printf("Pressed Up\n");
////                            shipy -= 2;
////                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ];
//                            break;
//                            case SDLK_DOWN:
//                            printf("Pressed Down\n");
////                            shipy += 2;
////                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ];
//                            break;
//                            case SDLK_LEFT:
////                            shipx -= 2;
//                            printf("Pressed Left\n");
////                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ];
//                            break;
//                            case SDLK_RIGHT:
////                            shipx += 2;
//                            printf("Pressed Right\n");
////                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ];
//                            break;
//                            case SDLK_h:
//                            printf("Pressed H key\n");
//                            break;
//                            case SDLK_ESCAPE:
//                            close();
//                            return 0;
//                            default:
//                            printf("Key pressed\n");
////                            gCurrentTexture = pngTexture;
//                            break;
//                        }
                }
                //Calculate the time step
                float timeStep = stepTimer.getTicks() / 1000.f;
                
                //Move for time step
                shipLTexture.move( timeStep);
                
                //Restart step stepTimer
                stepTimer.start();
                
                //Render the image
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, pngTexture, NULL, NULL);
                
                shipLTexture.render( (int)mPosX, (int)mPosY );
                //shipLTexture.render( 0, 0, &gSpriteClip );
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    //free resources and close SDL
    
    close();
    return 0;
}
