//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include <iostream>
#include "FarmUI.h"
#include "Camera.h"
#include "CreatureUI.h"
#include "FoodUI.h"
#include "colors/RGBColor.h"

FarmUI::FarmUI(Farm *farm, sf::Font * font): farm(farm), hoveredTile(Point(-1, -1)) {
    loadTexts(font);
}

void FarmUI::loadTexts(sf::Font * font) {
    hoveredTileInfos.setFont(*font);
}

void FarmUI::mouseMoved(Point worldPosition, Camera * camera) {
    hoveredTile = worldPosition.getTileCoordinates();

    Point topLeftChunk = Point(hoveredTile.getX() * TILE_SIZE, hoveredTile.getY() * TILE_SIZE);
    Point screenTopLeft = camera->getScreenCoordinates(topLeftChunk);


    hoveredTileInfos.setPosition(screenTopLeft.getX(), screenTopLeft.getY());
    hoveredTileInfos.setFillColor(sf::Color(100, 0, 0));
    hoveredTileInfos.setCharacterSize((5 / TILE_PER_CHUNK) * camera->getZoom());
    generateTileInfoText();
}

void FarmUI::generateTileInfoText() {

    std::string tileInfo = "{" + std::to_string(int(hoveredTile.getX())) + "," + std::to_string(int(hoveredTile.getY())) + "}\n";

    float currentHeight = farm->getMap()->getTileAt(hoveredTile.getX(), hoveredTile.getY());
    tileInfo = tileInfo + "H: " + std::to_string(currentHeight);



    hoveredTileInfos.setString(tileInfo);
}

void FarmUI::loadMap() {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2));
            rectangle.setPosition((float(it) * TILE_SIZE) + 1, (float(jt) * TILE_SIZE) + 1);


            line.emplace_back(rectangle);
        }
        tiles.emplace_back(line);
    }
}

void FarmUI::update() {
    // TODO OPTI Clear to_delete
    clearEntities(farm->getToDelete());

    addEntities(farm->getAddedEntity());
    farm->clearAddedEntities();

    addCreatures(farm->getAddedCreatures());
    farm->clearAddedCreatures();



    generateTileInfoText();
}

void FarmUI::setPositions(Camera *camera) {
//     std::cout << "Setting positions " << camera->getZoom() << std::endl;

    Map *map = farm->getMap();

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            sf::RectangleShape *rectangle = &tiles.at(it).at(jt);

            float height = map->getTileAt(it, jt);
            RGBColor rectangleColor = RGBColor(0.f, 0.f, ((height + 1) / 2));
            if (height < -0.1f) {
                rectangleColor = RGBColor(0.6f, 1.f, ((height + 1) / 2));
            }
            if (height > 0.7f) {
                rectangleColor = RGBColor(0.59f, 1.f, ((height + 1) / 2));
            }


            rectangle->setFillColor(sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255));

            int gridRatio = 0;

            if (camera->isShowGrid()) {
                gridRatio = 1;
            }

            rectangle->setSize(sf::Vector2f(TILE_SIZE * camera->getZoom() - gridRatio, TILE_SIZE * camera->getZoom() - gridRatio));

            Point point = camera->getScreenCoordinates({float(it) * TILE_SIZE, float(jt) * TILE_SIZE});

            rectangle->setPosition(point.getX(), point.getY());


        }
    }
}

void FarmUI::draw(sf::RenderWindow *window, Camera *camera, BrainConnector * selectedEntity) {
    Creature * selectedCreature = nullptr;


    setPositions(camera);

    if (selectedEntity != nullptr) {
        selectedCreature = selectedEntity->getCreature();
    }


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            window->draw(tiles.at(it).at(jt));
        }
    }

    for (int it = 0; it < entities.size(); it++) {
        entities.at(it)->draw(window, camera, selectedCreature);
    }

    window->draw(hoveredTileInfos);
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


