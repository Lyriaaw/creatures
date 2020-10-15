//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#ifndef CREATURES_COLORSSCREEN_H
#define CREATURES_COLORSSCREEN_H


#include "Screen.h"
#include "../dashboards/ColorsGraph.h"

class ColorsScreen: public Screen {
private:
    double windowWidth, windowHeight;

    ColorsGraph * colorsGraph;
public:
    ColorsScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height) override;



    void loadCamera();

    void mouseMoved(int x, int y) override;
};


#endif //CREATURES_COLORSSCREEN_H
