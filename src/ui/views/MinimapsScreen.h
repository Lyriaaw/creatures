//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_MINIMAPSSCREEN_H
#define CREATURES_MINIMAPSSCREEN_H


#include "Screen.h"
#include "../minimaps/Minimap.h"
#include "../minimaps/CreatureTileCountMinimap.h"
#include "../minimaps/FoodTileCountMinimap.h"
#include "../minimaps/HeatEnergyMinimap.h"
#include "../minimaps/GroundEnergyMinimap.h"

class MinimapsScreen: public Screen {
private:
    std::vector<Minimap *> minimaps;



    CreatureTileCountMinimap * creatureCountMinimap;
    FoodTileCountMinimap * foodTileCountMinimap;
    HeatEnergyMinimap * heatMinimap;
    GroundEnergyMinimap * groundMinimap;

    sf::RectangleShape background;

public:
    MinimapsScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height) override;

    void mouseMoved(int x, int y) override;

    void drawMinimaps(sf::RenderWindow *window);

    void placeMinimap(int x, int y, Minimap * minimap);
};


#endif //CREATURES_MINIMAPSSCREEN_H
