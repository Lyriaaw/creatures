//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include <iostream>
#include "FarmUI.h"
#include "Camera.h"

FarmUI::FarmUI() {}

void FarmUI::loadMap(Map *map) {
    this->map = map;

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(CHUNK_SIZE - 2, CHUNK_SIZE - 2));
            rectangle.setPosition((float(it) * CHUNK_SIZE) + 1, (float(jt) * CHUNK_SIZE) + 1);


            line.emplace_back(rectangle);
        }
        tiles.emplace_back(line);
    }
}

void FarmUI::setPositions(Camera *camera, Creature * selectedEntity) {
//     std::cout << "Setting positions " << camera->getZoom() << std::endl;

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            sf::RectangleShape *rectangle = &tiles.at(it).at(jt);

            float height = map->getTileAt(it, jt);
            if (height > 0) {
                rectangle->setFillColor(sf::Color(128, 128, 128, 255));
            } else {
                rectangle->setFillColor(sf::Color(0, 9, 128, 255));
            }

            if (selectedEntity != nullptr) {
                for (int kt = 0; kt < selectedEntity->getSelectedChunks().size(); kt++) {
                    if (selectedEntity->getSelectedChunks().at(kt).equals(Point(it, jt))) {
                        rectangle->setFillColor(sf::Color(200, 200, 200, 255));
                    }
                }
            }

            int gridRatio = 0;

            if (camera->isShowGrid()) {
                gridRatio = 1;
            }

            rectangle->setSize(sf::Vector2f(CHUNK_SIZE * camera->getZoom() - gridRatio, CHUNK_SIZE * camera->getZoom() - gridRatio));

            Point point = camera->getScreenCoordinates({float(it) * CHUNK_SIZE, float(jt) * CHUNK_SIZE});

            rectangle->setPosition(point.getX(), point.getY());


        }
    }
}

void FarmUI::draw(sf::RenderWindow *window, Camera *camera, Creature * selectedEntity) {
    setPositions(camera, selectedEntity);

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            window->draw(tiles.at(it).at(jt));
        }
    }

    for (int it = 0; it < entities.size(); it++) {
        Entity * currentEntity = entities.at(it)->getEntity();

        bool found = false;

        if (selectedEntity != nullptr) {
            for (int jt = 0; jt < selectedEntity->getAccessibleEntities().size(); jt++) {
                Entity * currentAccessibleEntity = selectedEntity->getAccessibleEntities().at(jt);
                if (currentAccessibleEntity == currentEntity) {
                    found = true;
                }
            }
        }

        if (found) {
            entities.at(it)->draw(window, camera, currentEntity);

        } else {
            entities.at(it)->draw(window, camera, selectedEntity);
        }
    }

}

void FarmUI::setEntities(const std::vector<EntityUI *> entities) {
    FarmUI::entities = entities;
}


