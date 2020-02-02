
//Using SDL and standard IO
#include "SDL2/SDL.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};
//Key press textures constants
enum KeyPressTextures
{
    KEY_PRESS_TEXTURE_DEFAULT,
    KEY_PRESS_TEXTURE_UP,
    KEY_PRESS_TEXTURE_DOWN,
    KEY_PRESS_TEXTURE_LEFT,
    KEY_PRESS_TEXTURE_RIGHT,
    KEY_PRESS_TEXTURE_TOTAL
};

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

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

//The SDL Renderer
SDL_Renderer *renderer;

//SDL Texture
SDL_Texture *texture;

//png Texture
SDL_Surface *pngSurface;
SDL_Texture *pngTexture;

//current displayed image
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* gCurrentSurface = NULL;

//Current Texture
SDL_Texture* gKeyPressTextures [ KEY_PRESS_TEXTURE_TOTAL ];
SDL_Texture* gCurrentTexture = NULL;

bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        success = false;
    }
    else
    {
        //Create the window
        if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) 
        {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return success = false;
        }
        else
        {
            //Get Window Surface
            surface = SDL_LoadBMP("sample.bmp");
            if (!surface) 
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
                return success = false;
            }
            texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
        {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return success = false;
        }
        SDL_FreeSurface(surface);
        }
    }
    return success;
}

bool loadMedia()
{
    //Loading Success flag
    bool success = true;
    
    //Load default surface from splash image
    surface = SDL_LoadBMP("sample.bmp");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    
    //Load default surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = SDL_LoadBMP("press.bmp");
    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ] = SDL_CreateTextureFromSurface(renderer, gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ]);
    if (gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ] == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    
        //Load up surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = SDL_LoadBMP("up.bmp");
    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ] = SDL_CreateTextureFromSurface(renderer, gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ]);
    if (gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ] == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    
        //Load down surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = SDL_LoadBMP("down.bmp");
    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ] = SDL_CreateTextureFromSurface(renderer, gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ]);
    if (gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ] == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    
        //Load left surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = SDL_LoadBMP("left.bmp");
    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ] = SDL_CreateTextureFromSurface(renderer, gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ]);
    if (gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ] == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    
        //Load right surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = SDL_LoadBMP("right.bmp");
    if ( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ] = SDL_CreateTextureFromSurface(renderer, gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ]);
    if (gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ] == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    
    //Initialize PNG Loading
    int imgFlags = IMG_INIT_PNG;
    if( !( 2 & imgFlags ) )
    {
        printf( "SDL_image could not initialize SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }
    pngSurface = IMG_Load("Assets/CloseHaulPixelArt.png");
    if ( pngSurface == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        success = false;
        return 3;
    }
    pngTexture = SDL_CreateTextureFromSurface(renderer, pngSurface);
    if(!pngTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    
    return success;
}

void close()
{
    //Deallocate surface
    SDL_FreeSurface(surface);
    
    //Destroy Window
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    //Quit SDL subsystems
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
            
            //Set default current surface
            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ];
        
            //While application is running
            while ( !quit )
            {
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
                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ];
                            break;
                            case SDLK_DOWN:
                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ];
                            break;
                            case SDLK_LEFT:
                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ];
                            break;
                            case SDLK_RIGHT:
                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ];
                            break;
                            case SDLK_h:
                            gCurrentTexture = pngTexture;
                            break;
                            case SDLK_ESCAPE:
                            close();
                            return 0;
                            default:
                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ];
                            break;
                        }
                }
                //Render the image
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, gCurrentTexture, NULL, NULL);
                SDL_RenderPresent(renderer);
            }
        }
    }
    //free resources and close SDL
    
    close();
    return 0;
}
