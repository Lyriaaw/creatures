//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_ENTITY_H
#define CREATURES_ENTITY_H


#include "Point.h"
#include <SDL2/SDL.h>


class Entity {
public:
    Entity(Point point, int size);
    void draw(SDL_Renderer *renderer);


private:
    Point point;
    int size;
};


#endif //CREATURES_ENTITY_H
