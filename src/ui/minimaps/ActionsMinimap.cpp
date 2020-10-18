//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "ActionsMinimap.h"

ActionsMinimap::ActionsMinimap(): Minimap() {}

ActionsMinimap::ActionsMinimap(double pixelSize, double positionX, double positionY)
        : Minimap(pixelSize, positionX, positionY) {


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            r[it][jt] = 0;
            g[it][jt] = 0;
            b[it][jt] = 0;
        }
    }
}



std::string ActionsMinimap::getName() {
    return "Heat";
}

void ActionsMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {

    sf::Color pixelColor = sf::Color(r[tileX][tileY], g[tileX][tileY], b[tileX][tileY], 255);

    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}

void ActionsMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}

sf::Color ActionsMinimap::getColorFromActionName(ActionDTO action) {
    if (action.getType() == "EAT_LIFE") {
        return sf::Color(255, 0, 0);
    }
    if (action.getType() == "EAT_ENTITY") {
        return sf::Color(0, 128, 0);
    }

    if (action.getType() == "BITE_LIFE") {
        return sf::Color(128, 128, 0);
    }
    if (action.getType() == "BITE_ENTITY") {
        return sf::Color(58, 0, 94);
    }

//    if (action.getType() == "POOP") {
//        return sf::Color(120, 148, 0);
//    }
    if (action.getType() == "MATE") {
        return sf::Color(255, 0, 174);
    }


    return sf::Color(0, 0, 0);
}

void ActionsMinimap::processRed() {
    double newColor[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            newColor[it][jt] = r[it][jt];
        }
    }



    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {


            float availableColor = r[it][jt] / 15;

            if (availableColor <= 10) {
                continue;
            }

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (it + x < 0 || it + x >= TILE_COUNT_WIDTH ||jt + y < 0 || jt + y >= TILE_COUNT_HEIGHT) {
                        continue;
                    }

                    newColor[it + x][jt + y] += availableColor;
                    newColor[it][jt] -= availableColor;

                }
            }

        }
    }


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            newColor[it][jt] -= std::min(1.0, newColor[it][jt]);
        }
    }


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            r[it][jt] = newColor[it][jt];
        }
    }
}

void ActionsMinimap::processGreen() {
    double newColor[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            newColor[it][jt] = g[it][jt];
        }
    }



    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {


            float availableColor = g[it][jt] / 15;

            if (availableColor <= 10) {
                continue;
            }

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (it + x < 0 || it + x >= TILE_COUNT_WIDTH ||jt + y < 0 || jt + y >= TILE_COUNT_HEIGHT) {
                        continue;
                    }

                    newColor[it + x][jt + y] += availableColor;
                    newColor[it][jt] -= availableColor;

                }
            }

        }
    }


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            newColor[it][jt] -= std::min(1.0, newColor[it][jt]);
        }
    }


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            g[it][jt] = newColor[it][jt];
        }
    }
}

void ActionsMinimap::processBlue() {
    double newColor[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            newColor[it][jt] = b[it][jt];
        }
    }



    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {


            float availableColor = b[it][jt] / 15;

            if (availableColor <= 10) {
                continue;
            }

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (it + x < 0 || it + x >= TILE_COUNT_WIDTH ||jt + y < 0 || jt + y >= TILE_COUNT_HEIGHT) {
                        continue;
                    }

                    newColor[it + x][jt + y] += availableColor;
                    newColor[it][jt] -= availableColor;

                }
            }

        }
    }


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            newColor[it][jt] -= std::min(1.0, newColor[it][jt]);
        }
    }


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            b[it][jt] = newColor[it][jt];
        }
    }
}





void ActionsMinimap::generateValues(Farm * farm) {
    std::vector<ActionDTO> currentActions = farm->getAndClearExecutedActions();

    for (int it = 0; it < currentActions.size(); it++) {
        ActionDTO currentAction = currentActions.at(it);

        if (farm->getTickCount() > currentAction.getTick() + 10) {
            continue;
        }

        sf::Color currentColor = getColorFromActionName(currentAction);

        int tileX = currentAction.getTilePosition().getX();
        int tileY = currentAction.getTilePosition().getY();

        this->r[tileX][tileY] = currentColor.r;
        this->g[tileX][tileY] = currentColor.g;
        this->b[tileX][tileY] = currentColor.b;

    }

    processRed();
    processGreen();
    processBlue();

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            colorGrid[it][jt] = sf::Color(r[it][jt], g[it][jt], b[it][jt]);
        }
    }
}

