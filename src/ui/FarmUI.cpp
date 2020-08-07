//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include <iostream>
#include "FarmUI.h"
#include "Camera.h"
#include "../farm/brain/BrainConnector.h"
#include "CreatureUI.h"
#include "FoodUI.h"

FarmUI::FarmUI(Farm *farm): farm(farm) {

}

void FarmUI::loadMap() {
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

void FarmUI::update() {

    clearEntities(farm->getToDelete());

    addEntities(farm->getAddedEntity());
    farm->clearAddedEntities();

    addCreatures(farm->getAddedCreatures());
    farm->clearAddedCreatures();



}

void FarmUI::setPositions(Camera *camera, Creature * selectedEntity) {
//     std::cout << "Setting positions " << camera->getZoom() << std::endl;

    Map *map = farm->getMap();

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

            if (selectedEntity != nullptr && camera->isShowGrid()) {
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

void FarmUI::draw(sf::RenderWindow *window, Camera *camera, BrainConnector * selectedEntity) {
    Creature * selectedCreature = nullptr;

    if (selectedEntity != nullptr) {
        selectedCreature = selectedEntity->getCreature();
    }

    setPositions(camera, selectedCreature);

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            window->draw(tiles.at(it).at(jt));
        }
    }

    for (int it = 0; it < entities.size(); it++) {
        entities.at(it)->draw(window, camera, selectedCreature);
    }

}

void FarmUI::clearEntities(std::vector<Entity *> toDelete) {
    for (int it = 0; it < toDelete.size(); it++) {
        Entity * entityToDelete = toDelete.at(it);

        int index = -1;
        for (int jt = 0; jt < entities.size(); jt++) {
            EntityUI * currentEntityUI = entities.at(jt);

            if (currentEntityUI->getEntity()->getId() == entityToDelete->getId()) {
                index = jt;
            }
        }

        if (index != -1) {
            EntityUI * toDeleteEntityUI = entities.at(it);
            entities.erase(entities.begin() + index);
        }

    }
}

void FarmUI::addEntities(std::vector<Food *> to_add) {
    for (int it = 0; it < to_add.size(); it++) {
        FoodUI *entityUi = new FoodUI(to_add.at(it));
        entities.push_back(entityUi);
    }
}
void FarmUI::addCreatures(std::vector<Creature *> to_add) {
    for (int it = 0; it < to_add.size(); it++) {
        CreatureUI *entityUi = new CreatureUI(to_add.at(it));
        entities.push_back(entityUi);
    }
}



void FarmUI::setEntities(const std::vector<EntityUI *> entities) {
    FarmUI::entities = entities;
}

void FarmUI::addCreature(CreatureUI * creatureUI) {
    this->entities.push_back(creatureUI);
}

const std::vector<EntityUI *> &FarmUI::getEntities() const {
    return entities;
}


