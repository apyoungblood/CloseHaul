//Using SDL and standard IO
#include "SDL2/SDL.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

//class Vector
//{
//public:
//    Vector(double,double);
//    double getX();
//    double getY();
//    double getSpeed();
//    double getAngle();
//    void setSpeed(double);
//    void setAngle(double);
//private:
//    double speed,angle;
//}
//
//Vector::Vector(double speed,double angle)
//{
//    this -> speed = speed;
//    this -> angle = angle;
//}
//
//double Vector::getX()
//{
//    return speed*cos(angle);
//}
//
//double Vector::getY()
//{
//    return speed*sin(angle);
//}
//
//double Vector::getSpeed()
//{
//    return speed;
//}
//
//double Vector::getAngle()
//{
//    return angle;
//}
//
//void Vector::setAngle(double angle)
//{
//    this -> angle = angle;
//}
//
//void Vector::setSpeed(double speed)
//{
//    this -> speed = speed;
//}
//
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

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL );

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

//class LSprite
//{
//    public:
//        LSprite(int x, int y);
//        SDL_Rect getRect();
//        SDL_Surface* getImage();
//        void setRect(SDL_Rect);
//        void move();
//        void draw(SDL_Surface*);
//        
//private:
//        Vector movement;
//        double x,y,lastX,lastY,angle,speed;
//        SDL_Rect rect;
//        SDL_Surface* image;
//}

//Start up SDL and create the window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL2
void close();

//The window to render to
SDL_Window *window;

//The surface contained by the window
SDL_Surface *surface;

//The SDL Renderer
SDL_Renderer *renderer;

//SDL Texture
SDL_Texture *texture;

//png Texture
SDL_Surface *pngSurface;
SDL_Texture *pngTexture;

//Ship Texture
SDL_Surface *shipSurface;
SDL_Texture *shipTexture;
//SDL_Window *shipWindow;
//SDL_Rect *shipRect;
//SDL_Renderer *shipRenderer;
//const int SHIP_HEIGHT = 95;
//const int SHIP_WIDTH = 108;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene sprites
//SDL_Rect gSpriteClips[ 4 ];
SDL_Rect gSpriteClip;
LTexture gSpriteSheetTexture;

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

void LTexture::render( int x, int y, SDL_Rect* clip )
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
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

//LSprite::LSprite(int x, int y): movement(1.0,0.0)
//{
//    this -> x = x
//    this -> y = y;
//    lastX = x;
//    lastY = y;
//    image = loadImage("Assets/Ship.png");
//    rect.x = x;
//    rect.y = y;
//    rect.w = image->w;
//    rect.h = image->h;
//    speed = 1;
//    angle = 0;
//}

//SDL_Rect LSprite::getRect()
//{
//    return rect;
//}
//
//SDL_Surface* LSprite::getImage()
//{
//    return image;
//}
//
//void LSprite::setRect(SDL_Rect rect)
//{
//    this -> rect = rect;
//}
//
//void LSprite::move()
//{
//    lastX = x;
//    lastY = y;
//    x += speed*cos(angle);
//    y += speed*sin(angle);
//    rect.x = int(x);
//    rect.y = int(y);
//}
//
//void LSprite::draw(SDL_Surface* dest)
//{
//    blit(image,dest,int(x),int(y));
//}


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
//    shipSurface = IMG_Load("Assets/Ship.png");
//    if (shipSurface == NULL )
//    {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s",SDL_GetError());
//        success = false;
//        return 3;
//    } 
//    shipTexture = SDL_CreateTextureFromSurface(renderer, shipSurface);
//    if(!shipTexture)
//    {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Couldn't create texture from surface %s",SDL_GetError());
//        success = false;
//        return 3;
//    }
//    SDL_FreeSurface(shipSurface);
    
    	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "Assets/Ship.png" ) )
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

void close()
{
    
    //Deallocate surface
    SDL_FreeSurface(surface);
    SDL_FreeSurface(pngSurface);
    SDL_FreeSurface(shipSurface);
    
    //Free loaded images
    gSpriteSheetTexture.free();
    
    //Destroy Window
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
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
        
            //Event handler
            SDL_Event event;
            
            //While application is running
            while ( !quit )
            {
                //x&y vars for ship sprite
                int shipx = 0;
                int shipy = 0;
                //Handle events on queue
                while ( SDL_PollEvent(&event) != 0 )
                {
                    //User requests quit
                    if (event.type == SDL_QUIT) 
                    {
                        quit = true;
                    }
                    //User presses a key
                    else if ( event.type == SDL_KEYDOWN )
                        //Select surfaces based on key press
                        switch( event.key.keysym.sym )
                        {
                            case SDLK_UP:
                            printf("Pressed Up\n");
//                            shipy -= 2;
//                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ];
                            break;
                            case SDLK_DOWN:
                            printf("Pressed Down\n");
//                            shipy += 2;
//                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ];
                            break;
                            case SDLK_LEFT:
//                            shipx -= 2;
                            printf("Pressed Left\n");
//                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ];
                            break;
                            case SDLK_RIGHT:
//                            shipx += 2;
                            printf("Pressed Right\n");
//                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ];
                            break;
                            case SDLK_h:
                            printf("Pressed H key\n");
                            break;
                            case SDLK_ESCAPE:
                            close();
                            return 0;
                            default:
                            printf("Key pressed\n");
//                            gCurrentTexture = pngTexture;
                            break;
                        }
                }
                //Render the image
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, pngTexture, NULL, NULL);
//                SDL_RenderCopy(renderer, shipTexture, NULL, shipRect);
                gSpriteSheetTexture.render( 0, 0, &gSpriteClip );
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    //free resources and close SDL
    
    close();
    return 0;
}
