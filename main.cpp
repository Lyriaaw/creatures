#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <tiff.h>
#include <random>
#include <array>
#include <ctime>

#include "src/Entity.h"

using namespace std;
using namespace std::chrono;

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;



void runLoop(SDL_Surface *mainSurface, SDL_Renderer *renderer) {

    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();

    vector<Entity> entities;


    for (int it = 0; it < 1000; it++) {
        random_device rd;
        mt19937 mt(rd());
        uniform_real_distribution<double> dist(0, WINDOW_HEIGHT);
        int x = dist(mt) + it;
        int y = dist(mt);

        Point point(x, y);

        Entity entity(point, 10);

    }




    bool done(false);
    SDL_Event events;
    while (!done) {



        while (SDL_PollEvent(&events) != 0) {
            switch (events.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_MOUSEMOTION:
                    cout << "Mouse moved to X: " << events.motion.x << " Y: " << events.motion.y << endl;
                    break;
                default:
                    break;
            }
        }



        SDL_Color color = {255, 255, 255};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

//        for (int it = 0; it < 1000; it++) {
//            move(&movableRect + it);
//        }

        for (int it = 0; it < 1000; it++) {
//            cout << "Drawing " << it << endl;
            Entity entity = entities.at(it);
            entity.draw(renderer);

//            drawRect(renderer, movableRect[it]);
        }

        chrono::duration<double> elapsed_time = start - end;

        short vxs[4] = {100, 200, 200, 100};
        short vys[4] = {200, 200, 100, 100};

        filledPolygonRGBA(mainSurface, vxs, vys, 5, 255, 0, 0, 255);

        SDL_RenderPresent(renderer);

        end = chrono::system_clock::now();
        cout << "Time : " << elapsed_time.count() << "s" << endl;


        start = chrono::system_clock::now();
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


    window = SDL_CreateWindow("Creatures", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

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
