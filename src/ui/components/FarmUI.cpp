//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include <iostream>
#include "FarmUI.h"
#include "Camera.h"
#include "../colors/RGBColor.h"

FarmUI::FarmUI(Farm *farm, sf::Font * font): farm(farm), hoveredTile(Point(-1, -1)), font(font) {
    loadTexts();
    loadFarm();
}


void FarmUI::loadTexts() {
    hoveredTileInfos.setFont(*font);
    hoveredTileInfos.setCharacterSize(0);
}


void FarmUI::loadFarm() {
    loadMap();
    loadLifes();
    loadEntities();
}

void FarmUI::loadMap() {
    tilesVertexArray = sf::VertexArray(sf::Quads, TILE_COUNT_WIDTH * TILE_COUNT_HEIGHT * 4);
}

void FarmUI::loadLifes() {
    std::vector<Life *> initialLifes = farm->fetchLifes();
    for (int it = 0; it < initialLifes.size(); it++) {
        Life * currentLife = initialLifes.at(it);

        LifeUI * ui = new LifeUI(currentLife, font);
        lifeUIs.emplace_back(ui);
    }
}

void FarmUI::loadEntities() {
    for (int it = 0; it < farm->getEntities().size(); it++) {
        Entity * currentEntity = farm->getEntities().at(it);

        EntityUI * ui = new EntityUI(currentEntity, font);
        entityUIs.emplace_back(ui);
    }
}


// =============


void FarmUI::addLife(Life * life) {
    LifeUI * ui = new LifeUI(life, font);
    lifeUIs.emplace_back(ui);
}

void FarmUI::addEntity(Entity * entity) {
    EntityUI * ui = new EntityUI(entity, font);
    entityUIs.emplace_back(ui);
}

void FarmUI::addLifes(std::vector<Life *> addedToFarm) {
    for (int it = 0; it < addedToFarm.size(); it++) {
        addLife(addedToFarm.at(it));
    }
}

void FarmUI::addEntities(std::vector<Entity *> addedToFarm) {
    for (int it = 0; it < addedToFarm.size(); it++) {
        addEntity(addedToFarm.at(it));
    }
}


void FarmUI::clearDeletedLifes(std::vector<Life *> deletedFromFarm) {
    for (int it = 0; it < deletedFromFarm.size(); it++) {
        Life * lifeToDelete = deletedFromFarm.at(it);

        int index = -1;
        for (int jt = 0; jt < lifeUIs.size(); jt++) {
            LifeUI * currentLife = lifeUIs.at(jt);

            if (currentLife->getLife()->getEntity()->getId() == lifeToDelete->getEntity()->getId()) {
                index = jt;
            }
        }

        if (index != -1) {
            lifeUIs.erase(lifeUIs.begin() + index);
        }

    }
}

void FarmUI::clearDeletedEntities(std::vector<Entity *> deletedFromFarm) {
    for (int it = 0; it < deletedFromFarm.size(); it++) {
        Entity * entityToDelete = deletedFromFarm.at(it);

        int index = -1;
        for (int jt = 0; jt < entityUIs.size(); jt++) {
            EntityUI * currentEntity = entityUIs.at(jt);

            if (currentEntity->getEntity()->getId() == entityToDelete->getId()) {
                index = jt;
            }
        }

        if (index != -1) {
            entityUIs.erase(entityUIs.begin() + index);
        }
    }
}


// =============


void FarmUI::generateTileInfoText() {

    if (hoveredTile.getX() < 0 || hoveredTile.getX() >= TILE_COUNT_WIDTH || hoveredTile.getY() < 0 || hoveredTile.getY() >= TILE_COUNT_HEIGHT) {
        return;
    }

    std::string tileInfo = "{" + std::to_string(int(hoveredTile.getX())) + "," + std::to_string(int(hoveredTile.getY())) + "}\n";

    float currentHeight = farm->getTileAt(hoveredTile.getX(), hoveredTile.getY())->getHeight();
    float currentHeat = farm->getTileAt(hoveredTile.getX(), hoveredTile.getY())->getHeat();
    float currentGround = farm->getTileAt(hoveredTile.getX(), hoveredTile.getY())->getGround();
    tileInfo = tileInfo + "Height: " + std::to_string(currentHeight) + "\n";
    tileInfo = tileInfo + "Heat: " + std::to_string(currentHeat) + "\n";
    tileInfo = tileInfo + "Ground: " + std::to_string(currentGround) + "\n";



    hoveredTileInfos.setString(tileInfo);
}



void FarmUI::update() {
    // TODO OPTI Clear to_delete

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            clearDeletedEntities(farm->getChunkAt(it, jt)->getEntityToDelete());
            farm->getChunkAt(it, jt)->clearToDeleteEntities();
            clearDeletedLifes(farm->getChunkAt(it, jt)->getLifesToDelete());
            farm->getChunkAt(it, jt)->clearToDeleteLifes();
            addLifes(farm->getChunkAt(it, jt)->getLifesAdded());
            farm->getChunkAt(it, jt)->clearAddedLifes();
            addEntities(farm->getChunkAt(it, jt)->getEntityAdded());
            farm->getChunkAt(it, jt)->clearAddedEntities();

        }
    }


    generateTileInfoText();
}

sf::Color FarmUI::getColorForTile(int x, int y, Camera * camera) {

    if (camera->getMapMode() == 0) {
        float height = farm->getTileAt(x, y)->getHeight();
        RGBColor rectangleColor = RGBColor(0.f, 0.f, ((height + 1) / 2));
        if (height < -0.05f) {
            rectangleColor = RGBColor(0.6f, 1.f, ((height + 1) / 2));
        }
        if (height > 0.7f) {
            rectangleColor = RGBColor(0.59f, 1.f, ((height + 1) / 2));
        }

        sf::Color tileColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);
        return tileColor;
    }

    if (camera->getMapMode() == 1) {
        float heat = farm->getTileAt(x, y)->getHeat() / 1000.f;
        RGBColor rectangleColor = RGBColor(0.f, 1.f, heat);

        if (heat > 1.f) {
            rectangleColor = RGBColor(0.68f, 1.f, heat - 1);
        }

        sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);
        return pixelColor;
    }

    if (camera->getMapMode() == 2) {
        float ground = farm->getTileAt(x, y)->getGround();

        RGBColor rectangleColor = RGBColor(0.28f, 1.f, ground / 10000.f);
        sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);

        return pixelColor;
    }


    return sf::Color(0, 0, 0, 255);


}

void FarmUI::setPositions(Camera *camera, Life * selectedEntity) {
//     std::cout << "Setting positions " << camera->getZoom() << std::endl;

    if (camera == nullptr) {
        return;
    }


//    Map *map = farm->getMap();

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Point chunkPoint = Point(it, jt);
            Point chunkTopLeft = Point((it * TILE_SIZE), (jt * TILE_SIZE));
            Point chunkBottomRight = Point((it * TILE_SIZE) + TILE_SIZE, (jt * TILE_SIZE) + TILE_SIZE);
            Point chunkTopRight = Point((it * TILE_SIZE) + TILE_SIZE, jt * TILE_SIZE);
            Point chunkBottomLeft = Point((it * TILE_SIZE), (jt * TILE_SIZE) + TILE_SIZE);


            if (!camera->shouldDisplayPoint(camera->getScreenCoordinates(chunkTopLeft)) &&
                    !camera->shouldDisplayPoint(camera->getScreenCoordinates(chunkBottomRight)) &&
                    !camera->shouldDisplayPoint(camera->getScreenCoordinates(chunkTopRight)) &&
                    !camera->shouldDisplayPoint(camera->getScreenCoordinates(chunkBottomLeft))
                    ) {
                int vertexArrayIndex = ((it * TILE_COUNT_HEIGHT) + jt) * 4;

                tilesVertexArray[vertexArrayIndex + 0].position = sf::Vector2f(-10, -10);
                tilesVertexArray[vertexArrayIndex + 1].position = sf::Vector2f(-10, -10);
                tilesVertexArray[vertexArrayIndex + 2].position = sf::Vector2f(-10, -10);
                tilesVertexArray[vertexArrayIndex + 3].position = sf::Vector2f(-10, -10);
                continue;
            }

            int topLeftRatio = 0;
            int rightRatio = 0;
            int bottomRatio = 0;

            if (camera->isShowGrid()) {
                topLeftRatio = 1;

                if (it % TILE_PER_CHUNK == TILE_PER_CHUNK - 1) {
                    rightRatio = 1;
                }
                if (jt % TILE_PER_CHUNK == TILE_PER_CHUNK - 1) {
                    bottomRatio = 1;
                }

            }

            int vertexArrayIndex = ((it * TILE_COUNT_HEIGHT) + jt) * 4;
            Point point = camera->getScreenCoordinates({float(it) * TILE_SIZE, float(jt) * TILE_SIZE});
            tilesVertexArray[vertexArrayIndex + 0].position = sf::Vector2f(
                    point.getX() + topLeftRatio,
                    point.getY() + topLeftRatio);


            tilesVertexArray[vertexArrayIndex + 1].position = sf::Vector2f(
                    point.getX() + (TILE_SIZE * camera->getZoom()) - rightRatio,
                    point.getY() + topLeftRatio);


            tilesVertexArray[vertexArrayIndex + 2].position = sf::Vector2f(
                    point.getX() + (TILE_SIZE * camera->getZoom()) - rightRatio,
                    point.getY() + (TILE_SIZE * camera->getZoom()) - bottomRatio);


            tilesVertexArray[vertexArrayIndex + 3].position = sf::Vector2f(
                    point.getX() + topLeftRatio,
                    point.getY() + (TILE_SIZE * camera->getZoom()) - bottomRatio);





            sf::Color tileColor = getColorForTile(it, jt, camera);
//            if (selectedEntity != nullptr) {
//                for (int kt = 0; kt < selectedEntity->getSelectedTiles().size(); kt++) {
//                    if (selectedEntity->getSelectedTiles().at(kt).equals(Point(it, jt))) {
//                        tileColor = sf::Color(55, 55, 55, 255);
//                        kt = selectedEntity->getSelectedTiles().size();
//                    }
//                }
//            }

            tilesVertexArray[vertexArrayIndex + 0].color = tileColor;
            tilesVertexArray[vertexArrayIndex + 1].color = tileColor;
            tilesVertexArray[vertexArrayIndex + 2].color = tileColor;
            tilesVertexArray[vertexArrayIndex + 3].color = tileColor;



        }
    }
}

void FarmUI::draw(sf::RenderWindow *window, Camera *camera, Life * selectedEntity) {
    Entity * selected = nullptr;

    if (selectedEntity != nullptr) {
        selected = selectedEntity->getEntity();
        camera->setCenter(selected->getPosition());
    }



    setPositions(camera, selectedEntity);

    window->draw(tilesVertexArray);
    for (int it = 0; it < entityUIs.size(); it++) {
        entityUIs.at(it)->draw(window, camera, selected);
    }

    for (int it = 0; it < lifeUIs.size(); it++) {
        lifeUIs.at(it)->draw(window, camera, selected);
    }



    window->draw(hoveredTileInfos);
}





void FarmUI::mouseMoved(Point worldPosition, Camera * camera) {
    hoveredTile = worldPosition.getTileCoordinates();

    Point topLeftChunk = Point(hoveredTile.getX() * TILE_SIZE, hoveredTile.getY() * TILE_SIZE);
    Point screenTopLeft = camera->getScreenCoordinates(topLeftChunk);


    hoveredTileInfos.setPosition(screenTopLeft.getX(), screenTopLeft.getY());
    hoveredTileInfos.setFillColor(sf::Color(100, 0, 0));
    hoveredTileInfos.setCharacterSize((TILE_SIZE / TILE_PER_CHUNK) * camera->getZoom());
    generateTileInfoText();
}


