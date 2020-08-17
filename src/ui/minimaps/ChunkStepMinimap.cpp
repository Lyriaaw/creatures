//
// Created by Amalric Lombard de Buffières on 8/17/20.
//


int TMP_WINDOW_WIDTH = 2560;
int TMP_WINDOW_HEIGHT = 1440;


#include "ChunkStepMinimap.h"


ChunkStepMinimap::ChunkStepMinimap(sf::Font * font) {

    steps.emplace_back("READY_TO_START");
    colors.emplace_back(sf::Color(255, 255, 255, 255));

    steps.emplace_back("ENTITY_GRID");
    colors.emplace_back(sf::Color(55, 0, 0, 255));

    steps.emplace_back("ENERGY_GIVEAWAY");
    colors.emplace_back(sf::Color(255, 0, 0, 255));

    steps.emplace_back("CLIMATE_START");
    colors.emplace_back(sf::Color(0, 0, 55, 255));

    steps.emplace_back("CLIMATE_READY");
    colors.emplace_back(sf::Color(0, 0, 100, 255));

    steps.emplace_back("CLIMATE_SPREAD");
    colors.emplace_back(sf::Color(0, 0, 200, 255));

    steps.emplace_back("BRAIN_PROCESSING");
    colors.emplace_back(sf::Color(0, 0, 0, 255));

    steps.emplace_back("EXECUTE_ACTIONS");
    colors.emplace_back(sf::Color(55, 0, 55, 255));

    steps.emplace_back("MOVE_CREATURES");
    colors.emplace_back(sf::Color(100, 100, 100, 255));

    steps.emplace_back("STATISTICS");
    colors.emplace_back(sf::Color(0, 100, 0, 255));

    steps.emplace_back("TICK_PASS");
    colors.emplace_back(sf::Color(0, 200, 0, 255));




    for (int it = 0; it < steps.size(); it++) {
        sf::Text text;
        text.setFont(*font);
        text.setCharacterSize(15);
        text.setString(steps.at(it));
        text.setFillColor(colors.at(it));
        text.setPosition(TMP_WINDOW_WIDTH - text.getGlobalBounds().width - 10, 0.1 * TMP_WINDOW_HEIGHT + (it * 20));
        legends.emplace_back(text);
    }

}

void ChunkStepMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {

    Chunk * chunk = farm->getChunkAt(tileX / TILE_PER_CHUNK, tileY / TILE_PER_CHUNK);

    std::string currentChunkStep = chunk->getStep();

    sf::Color pixelColor = sf::Color(0, 0, 0, 255);


    for (int it = 0; it < steps.size(); it++) {
        if (steps.at(it) == currentChunkStep) {
            pixelColor = colors.at(it);
        }
    }




    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}


void ChunkStepMinimap::draw(sf::RenderWindow *window) {

    for (int it = 0; it < steps.size(); it++) {
        window->draw(legends.at(it));
    }

    window->draw(vertexArray);
}






std::string ChunkStepMinimap::getName() {
    return "Chunk steps";
}
