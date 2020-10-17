//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_WORLDSCREEN_H
#define CREATURES_WORLDSCREEN_H


#include "Screen.h"

class WorldScreen: public Screen {
private:
    Camera *camera;
public:
    WorldScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height);



    void loadCamera();

    void mouseMoved(int x, int y);
};


#endif //CREATURES_WORLDSCREEN_H
