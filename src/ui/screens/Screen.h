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

    bool rightMouseButtonDown, leftMouseButtonDown;

public:
    Screen(FarmUI * farmUi);

    virtual int getId() = 0;

    virtual void init() = 0;
    virtual void updateSelectedCreature() = 0;

    virtual void draw(sf::RenderWindow *window);
    virtual void onWindowResize(int width, int height);

    virtual void mouseMoved(int x, int y, int previousX, int previousY);

    virtual void mouseClicked(int x, int y);

    void setMouseButtons(bool left, bool right);

    virtual void mouseScrolled(float delta, int mouseX, int mouseY);
};


#endif //CREATURES_SCREEN_H
