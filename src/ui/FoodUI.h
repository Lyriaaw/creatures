//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#ifndef CREATURES_FOODUI_H
#define CREATURES_FOODUI_H


#include "EntityUI.h"
#include "../farm/Food.h"

class FoodUI: public EntityUI {
public:
    FoodUI(Food *food);
    void draw(sf::RenderWindow *window, Camera *camera);
};


#endif //CREATURES_FOODUI_H
