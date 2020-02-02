//Using SDL and standard IO
#include "SDL2/SDL.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>

//Screen dimension constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

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

//Ship Texture
SDL_Surface *shipSurface;
SDL_Texture *shipTexture;
SDL_Window *shipWindow;
SDL_Rect *shipRect;
SDL_Renderer *shipRenderer;
const int SHIP_HEIGHT = 95;
const int SHIP_WIDTH = 108;

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
    pngTexture = SDL_CreateTextureFromSurface(renderer, pngSurface);
    if(!pngTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        success = false;
        return 3;
    }
    SDL_FreeSurface(pngSurface);
    shipSurface = IMG_Load("Assets/Ship.png");
    if (shipSurface == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s",SDL_GetError());
        success = false;
        return 3;
    } 
    shipTexture = SDL_CreateTextureFromSurface(renderer, shipSurface);
    if(!shipTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Couldn't create texture from surface %s",SDL_GetError());
        success = false;
        return 3;
    }
    SDL_FreeSurface(shipSurface);
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
                            printf("Pressed Up\n");
//                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ];
                            break;
                            case SDLK_DOWN:
                            printf("Pressed Down\n");
//                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ];
                            break;
                            case SDLK_LEFT:
                            printf("Pressed Left\n");
//                            gCurrentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ];
                            break;
                            case SDLK_RIGHT:
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
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, pngTexture, NULL, NULL);
                SDL_RenderCopy(renderer, shipTexture, NULL, shipRect);
                SDL_RenderPresent(renderer);
            }
        }
    }
    //free resources and close SDL
    
    close();
    return 0;
}
