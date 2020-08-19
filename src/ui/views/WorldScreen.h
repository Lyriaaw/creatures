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

    void draw(sf::RenderWindow *window) override;

    void updateSelectedLife(Life * life) override;

    void onWindowResize(int width, int height) override;



    void loadCamera();

    void mouseMoved(int x, int y) override;
};


#endif //CREATURES_WORLDSCREEN_H
