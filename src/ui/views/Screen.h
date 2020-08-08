//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_SCREEN_H
#define CREATURES_SCREEN_H


#include <SFML/Graphics.hpp>
#include "../Camera.h"
#include "../../farm/brain/BrainConnector.h"

class Screen {
private:
    int id;

    BrainConnector * selectedEntity;

public:
    Screen();

    virtual int getId() = 0;

    virtual void init() = 0;
    virtual Camera *open() = 0;
    virtual void draw(sf::RenderWindow *window) = 0;
    virtual void updateSelectedCreature(BrainConnector * connector) = 0;
    virtual void onWindowResize(int width, int height) = 0;
    virtual void mouseMoved(int x, int y) = 0;
};


#endif //CREATURES_SCREEN_H
