//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "MinimapsScreen.h"
#include "../minimaps/CreatureTileCountMinimap.h"

MinimapsScreen::MinimapsScreen(Farm *farm, sf::Font * font) : Screen(farm, font) {}

int MinimapsScreen::getId() {
    return 3;
}



void MinimapsScreen::init() {
    WorldMinimap * worldMinimap = new WorldMinimap();
    placeMinimap(0, 0, worldMinimap);

    creatureCountMinimap = new CreatureTileCountMinimap();
    placeMinimap( 1, 0, creatureCountMinimap);


    foodTileCountMinimap = new FoodTileCountMinimap();
    placeMinimap( 2, 0, foodTileCountMinimap);


    heatMinimap = new HeatEnergyMinimap();
    placeMinimap(0, 1, heatMinimap);

    groundMinimap = new GroundEnergyMinimap();
    placeMinimap(2, 1, groundMinimap);

    actionsMinimap = new ActionsMinimap();
    placeMinimap(1, 1, actionsMinimap);





    background = sf::RectangleShape(sf::Vector2f(0, 0));
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(50, 50, 50, 255));
}

void MinimapsScreen::placeMinimap(int x, int y, Minimap * minimap) {
    minimap->move(((x * (TILE_COUNT_WIDTH * minimap->getPixelSize()))) + (x * 10), 70 + ((y * (TILE_COUNT_HEIGHT * minimap->getPixelSize())) + 10) + (y * 10), minimap->getPixelSize() * TILE_COUNT_WIDTH, minimap->getPixelSize() * TILE_COUNT_HEIGHT);
    minimaps.emplace_back(minimap);
}

Camera *MinimapsScreen::open() {
    return nullptr;
}

void MinimapsScreen::drawMinimaps(sf::RenderWindow *window) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            for (int kt = 0; kt < minimaps.size(); kt++) {
                minimaps.at(kt)->setPixelColor(it, jt, farm);
            }

        }
    }


    for (int kt = 0; kt < minimaps.size(); kt++) {
        minimaps.at(kt)->draw(window);
    }

}



void MinimapsScreen::draw(sf::RenderWindow *window) {
    creatureCountMinimap->generateValues(farm);
    foodTileCountMinimap->generateValues(farm);
    heatMinimap->generateValues(farm);
    groundMinimap->generateValues(farm);
    actionsMinimap->generateValues(farm);
    window->draw(background);
    drawMinimaps(window);
}

void MinimapsScreen::updateSelectedCreature(Life *connector) {

}

void MinimapsScreen::onWindowResize(int width, int height) {
    background.setSize(sf::Vector2f(width, height));

}

void MinimapsScreen::mouseMoved(int x, int y) {

}
