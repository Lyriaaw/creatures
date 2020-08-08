//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_MINIMAPSSCREEN_H
#define CREATURES_MINIMAPSSCREEN_H


#include "Screen.h"
#include "../minimaps/Minimap.h"

class MinimapsScreen: public Screen {
private:
    std::vector<Minimap *> tileMinimaps;
    std::vector<Minimap *> chunkMinimaps;

public:
    MinimapsScreen(Farm *farm);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(BrainConnector *connector) override;

    void onWindowResize(int width, int height) override;

    void mouseMoved(int x, int y) override;

    void drawMinimaps(sf::RenderWindow *window);
};


#endif //CREATURES_MINIMAPSSCREEN_H
