//
// Created by Amalric Lombard de Buffières on 10/10/20.
//

#ifndef CREATURES_CREATURESCREEN_H
#define CREATURES_CREATURESCREEN_H


#include "Screen.h"

class CreatureScreen: public Screen {
private:
    Camera *camera;
public:
    CreatureScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height) override;



    void loadCamera();

    void mouseMoved(int x, int y) override;
};


#endif //CREATURES_CREATURESCREEN_H
