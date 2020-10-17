//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_SCREEN_H
#define CREATURES_SCREEN_H


#include <SFML/Graphics.hpp>
#include "../components/Camera.h"
#include "../../farm/brain/BrainConnector.h"
#include "../../farm/Farm.h"
#include "../elements/UiComponent.h"

class Screen {
protected:
    int id;

    Life * selectedEntity;

    Farm * farm;
    sf::Font * font;

    std::vector<UiComponent *> uiComponents;

public:
    Screen(Farm * farm, sf::Font * font);

    virtual int getId() = 0;

    virtual void init() = 0;
    virtual Camera *open() = 0;
    virtual void updateSelectedCreature(Life * connector) = 0;

    void draw(sf::RenderWindow *window);
    void onWindowResize(int width, int height);
    void mouseMoved(int x, int y);
    void mouseClicked(int x, int y);
};


#endif //CREATURES_SCREEN_H
