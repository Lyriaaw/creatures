//
// Created by Amalric Lombard de Buffières on 8/17/20.
//

#include "ChunkStepMinimap.h"


ChunkStepMinimap::ChunkStepMinimap() {
}

void ChunkStepMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {

    Chunk * chunk = farm->getChunkAt(tileX / TILE_PER_CHUNK, tileY / TILE_PER_CHUNK);

    std::string currentChunkStep = chunk->getStep();

    sf::Color pixelColor = sf::Color(0, 0, 0, 255);

    if ("ENTITY_GRID") {
        pixelColor = sf::Color(55, 55, 55, 255);
    }
    if ("ENERGY_GIVEAWAY") {
        pixelColor = sf::Color(255, 0, 0, 255);
    }
    if ("CLIMATE_START") {
        pixelColor = sf::Color(0, 55, 0, 255);
    }
    if ("CLIMATE_READY") {
        pixelColor = sf::Color(0, 100, 0, 255);
    }
    if ("CLIMATE_SPREAD") {
        pixelColor = sf::Color(55, 100, 0, 255);
    }
    if ("BRAIN_PROCESSING") {
        pixelColor = sf::Color(0, 0, 55, 255);
    }
    if ("EXECUTE_ACTIONS") {
        pixelColor = sf::Color(0, 0, 100, 255);
    }
    if ("MOVE_CREATURES") {
        pixelColor = sf::Color(100, 0, 100, 255);
    }


    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}


void ChunkStepMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}



std::string ChunkStepMinimap::getName() {
    return "Chunk steps";
}
