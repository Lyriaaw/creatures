//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "MinimapsScreen.h"

MinimapsScreen::MinimapsScreen(Farm *farm) : Screen(farm) {}

int MinimapsScreen::getId() {
    return 3;
}



void MinimapsScreen::init() {
    WorldMinimap * worldMinimap = new WorldMinimap(2, 10, 70);
    tileMinimaps.emplace_back(worldMinimap);

    CreatureCountMinimap * creatureCountMinimap = new CreatureCountMinimap(4, 20 + TILE_COUNT_WIDTH * 2, 70);
    chunkMinimaps.emplace_back(creatureCountMinimap);



}

Camera *MinimapsScreen::open() {
    return nullptr;
}

void MinimapsScreen::drawMinimaps(sf::RenderWindow *window) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            for (int kt = 0; kt < tileMinimaps.size(); kt++) {
                tileMinimaps.at(kt)->draw(it, jt, farm, window);
            }

        }
    }

     for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            for (int kt = 0; kt < chunkMinimaps.size(); kt++) {
                chunkMinimaps.at(kt)->draw(it, jt, farm, window);
            }

        }
    }


}



void MinimapsScreen::draw(sf::RenderWindow *window) {
    drawMinimaps(window);
}

void MinimapsScreen::updateSelectedCreature(BrainConnector *connector) {

}

void MinimapsScreen::onWindowResize(int width, int height) {
}

void MinimapsScreen::mouseMoved(int x, int y) {

}
