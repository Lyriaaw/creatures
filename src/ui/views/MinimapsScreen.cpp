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
    WorldMinimap * worldMinimap = new WorldMinimap(2, 10, 70);
    minimaps.emplace_back(worldMinimap);

    creatureCountMinimap = new CreatureTileCountMinimap(2, 20 + (TILE_COUNT_WIDTH * 1) * 2, 70);
    minimaps.emplace_back(creatureCountMinimap);

    foodTileCountMinimap = new FoodTileCountMinimap(2, 30 + (TILE_COUNT_WIDTH * 2) * 2, 70);
    minimaps.emplace_back(foodTileCountMinimap);

    heatMinimap = new HeatEnergyMinimap(2, 10, 70 + (TILE_COUNT_HEIGHT * 2) + 30);
    minimaps.emplace_back(heatMinimap);

    groundMinimap = new GroundEnergyMinimap(2, 20 + (TILE_COUNT_WIDTH * 1) * 2, 70 + (TILE_COUNT_HEIGHT * 2) + 30);
    minimaps.emplace_back(groundMinimap);



    background = sf::RectangleShape(sf::Vector2f(0, 0));
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(10, 10, 10, 255));
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
    window->draw(background);
    drawMinimaps(window);
}

void MinimapsScreen::updateSelectedCreature(BrainConnector *connector) {

}

void MinimapsScreen::onWindowResize(int width, int height) {
    background.setSize(sf::Vector2f(width, height));
}

void MinimapsScreen::mouseMoved(int x, int y) {

}
