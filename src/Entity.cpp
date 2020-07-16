//
// Created by Amalric Lombard de Buffières on 7/16/20.
//
#include <SDL2/SDL.h>

#include "Entity.h"


Entity::Entity(Point point, int size): point(point) {
    this->size = size;
}

void Entity::draw(SDL_Renderer *renderer) {

}
