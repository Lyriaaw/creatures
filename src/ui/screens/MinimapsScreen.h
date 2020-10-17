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
#include "../minimaps/ActionsMinimap.h"

class MinimapsScreen: public Screen {
private:
    std::vector<Minimap *> minimaps;



    CreatureTileCountMinimap * creatureCountMinimap;
    FoodTileCountMinimap * foodTileCountMinimap;
    HeatEnergyMinimap * heatMinimap;
    GroundEnergyMinimap * groundMinimap;
    ActionsMinimap * actionsMinimap;

    sf::RectangleShape background;

public:
    MinimapsScreen(FarmUI * farmUi);

    int getId() override;

    void init() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature() override;

    void onWindowResize(int width, int height);

    void mouseMoved(int x, int y);

    void drawMinimaps(sf::RenderWindow *window);

    void placeMinimap(int x, int y, Minimap * minimap);
};


#endif //CREATURES_MINIMAPSSCREEN_H
