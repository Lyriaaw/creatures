#include <iostream>

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;



void runLoop(SDL_Surface *mainSurface, SDL_Renderer *renderer) {

    bool done(false);
    SDL_Event events;
    while (!done) {
        while (SDL_PollEvent(&events) != 0) {
            if (events.type == SDL_QUIT) {
                done = true;
            }
        }

        SDL_Color color = {255, 255, 255};

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderClear(renderer);





        SDL_RenderPresent(renderer);
    }

}

int main(int argc, char **argv)
{
    SDL_Window* window(nullptr);
    SDL_Surface *mainSurface(nullptr);
    SDL_Renderer *renderer(nullptr);



    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Error while SDL init : " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }


    window = SDL_CreateWindow("Creatures", 0, 0, 800, 600, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        cout << "Error while creating the window" << endl;
        SDL_Quit();
        return -1;
    }


    mainSurface = SDL_GetWindowSurface(window);

    if (mainSurface == nullptr) {
        cout << "Error while creating the main surface" << endl;
        SDL_Quit();
        return -1;
    }


    renderer = SDL_GetRenderer(window);
    if (renderer == nullptr) {
        cout << "Error while creating the renderer: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }


    runLoop(mainSurface, renderer);



    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
