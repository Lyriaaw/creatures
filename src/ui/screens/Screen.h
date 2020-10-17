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
#include "../components/FarmUI.h"

class Screen {
protected:

    Life * selectedEntity;

    sf::Font * font;


    int id;
    std::vector<UiComponent *> uiComponents;
    FarmUI * farmUi;

public:
    Screen(FarmUI * farmUi);

    virtual int getId() = 0;

    virtual void init() = 0;
    virtual void updateSelectedCreature(Life * connector) = 0;

    virtual void draw(sf::RenderWindow *window);
    virtual void onWindowResize(int width, int height);
    void mouseMoved(int x, int y);
    void mouseClicked(int x, int y);
};


#endif //CREATURES_SCREEN_H
