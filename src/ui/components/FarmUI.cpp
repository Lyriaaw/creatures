//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include <iostream>
#include <sstream>

#include "FarmUI.h"
#include "Camera.h"
#include "../colors/RGBColor.h"
#include "../elements/GlobalFont.h"

FarmUI::FarmUI(Farm *farm): farm(farm), hoveredTile(Point(-1, -1)) {
    loadTexts();
    loadFarm();
}


void FarmUI::loadTexts() {
    hoveredTileInfos.setFont(*GlobalFont::MainFont);
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
    for (int it = 0; it < farm->getLifes().size(); it++) {
        Life * currentLife = farm->getLifes().at(it);

        LifeUI * ui = new LifeUI(currentLife, GlobalFont::MainFont);
        lifeUIs.emplace_back(ui);
    }
}

void FarmUI::loadEntities() {
    for (int it = 0; it < farm->getEntities().size(); it++) {
        Entity * currentEntity = farm->getEntities().at(it);

        EntityUI * ui = new EntityUI(currentEntity, GlobalFont::MainFont);
        entityUIs.emplace_back(ui);
    }
}


// =============


void FarmUI::addLife(Life * life) {
    LifeUI * ui = new LifeUI(life, GlobalFont::MainFont);
    lifeUIs.emplace_back(ui);
}

void FarmUI::addEntity(Entity * entity) {
    EntityUI * ui = new EntityUI(entity, GlobalFont::MainFont);
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

    Point currentHoveredTile = hoveredTile;

    if (currentHoveredTile.getX() < 0 || currentHoveredTile.getX() >= TILE_COUNT_WIDTH || currentHoveredTile.getY() < 0 || currentHoveredTile.getY() >= TILE_COUNT_HEIGHT) {
        return;
    }

    std::string tileInfo = "{" + std::to_string(int(currentHoveredTile.getX())) + "," + std::to_string(int(currentHoveredTile.getY())) + "}\n";

    float currentHeight = farm->getMap()->getTileAt(currentHoveredTile.getX(), currentHoveredTile.getY())->getHeight();
    float currentHeat = farm->getMap()->getTileAt(currentHoveredTile.getX(), currentHoveredTile.getY())->getHeat();
    float currentGround = farm->getMap()->getTileAt(currentHoveredTile.getX(), currentHoveredTile.getY())->getGround();
    float currentColor = farm->getMap()->getTileAt(currentHoveredTile.getX(), currentHoveredTile.getY())->getColor();
    tileInfo = tileInfo + "Height: " + std::to_string(currentHeight) + "\n";
    tileInfo = tileInfo + "Color: " + std::to_string(currentColor) + "\n";
    tileInfo = tileInfo + "Heat: " + std::to_string(currentHeat) + "\n";
    tileInfo = tileInfo + "Ground: " + std::to_string(currentGround) + "\n";



    hoveredTileInfos.setString(tileInfo);
}



void FarmUI::update() {
    // TODO OPTI Clear to_delete

    clearDeletedEntities(farm->getAndClearEntitiesToDelete());

    clearDeletedLifes(farm->getAndClearLifesToDelete());

    addLifes(farm->getAndClearLifesToAdd());

    addEntities(farm->getAndClearEntitiesToAdd());

    generateTileInfoText();
}

sf::Color FarmUI::getColorForTile(int x, int y, Camera * camera, Map *map) {

    if (camera->getMapMode() == 0) {
        float height = map->getTileAt(x, y)->getHeight();
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
        float heat = map->getTileAt(x, y)->getHeat() / 1000.f;
        RGBColor rectangleColor = RGBColor(0.f, 1.f, heat);

        if (heat > 1.f) {
            rectangleColor = RGBColor(0.68f, 1.f, heat - 1);
        }

        sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);
        return pixelColor;
    }

    if (camera->getMapMode() == 2) {
        float ground = map->getTileAt(x, y)->getGround();

        RGBColor rectangleColor = RGBColor(0.28f, 1.f, ground / 10000.f);
        sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);

        return pixelColor;
    }

    if (camera->getMapMode() == 3) {
        float ground = map->getTileAt(x, y)->getColor();

        RGBColor rectangleColor = RGBColor(ground, 1.f, 0.2f);
        sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);

        return pixelColor;
    }




    return sf::Color(0, 0, 0, 255);


}

void FarmUI::setPositions(Camera *camera) {
//     std::cout << "Setting positions " << camera->getZoom() << std::endl;

    if (camera == nullptr) {
        return;
    }

    Map *map = farm->getMap();

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


            int gridRatio = 0;

            if (camera->isShowGrid()) {
                gridRatio = 1;
            }

            int vertexArrayIndex = ((it * TILE_COUNT_HEIGHT) + jt) * 4;
            Point point = camera->getScreenCoordinates({float(it) * TILE_SIZE, float(jt) * TILE_SIZE});
            tilesVertexArray[vertexArrayIndex + 0].position = sf::Vector2f(
                    std::max(double(point.getX()), double(camera->getTopLeft().getX())),
                    std::max(double(point.getY()), double(camera->getTopLeft().getY())));


            double currentX = point.getX() + (TILE_SIZE * camera->getZoom()) - gridRatio;
            double currentY = point.getY() + (TILE_SIZE * camera->getZoom()) - gridRatio;
            tilesVertexArray[vertexArrayIndex + 1].position = sf::Vector2f(
                    std::min(double(currentX), double(camera->getTopLeft().getX()) + double(camera->getWidth())),
                    std::max(double(point.getY()), double(camera->getTopLeft().getY())));


            tilesVertexArray[vertexArrayIndex + 2].position = sf::Vector2f(
                    std::min(double(currentX), double(camera->getTopLeft().getX()) + double(camera->getWidth())),
                    std::min(double(currentY), double(camera->getTopLeft().getY()) + double(camera->getHeight())));


            tilesVertexArray[vertexArrayIndex + 3].position = sf::Vector2f(
                    std::max(double(point.getX()), double(camera->getTopLeft().getX())),
                    std::min(double(currentY), double(camera->getTopLeft().getY()) + double(camera->getHeight())));




            sf::Color tileColor = getColorForTile(it, jt, camera, map);
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


    std::vector<EntityUI *> currentEntities = entityUIs;

    std::vector<LifeUI *> currentLifes = lifeUIs;
    setPositions(camera);

    window->draw(tilesVertexArray);
    for (int it = 0; it < currentEntities.size(); it++) {
        currentEntities.at(it)->draw(window, camera, selected);
    }

    for (int it = 0; it < currentLifes.size(); it++) {
        currentLifes.at(it)->draw(window, camera, selected);
    }

    Point worldCoordinates = Point(hoveredTile.getX() * TILE_SIZE, hoveredTile.getY() * TILE_SIZE);

    if (camera->shouldDisplayPoint(camera->getScreenCoordinates(worldCoordinates))) {
        window->draw(hoveredTileInfos);
    }


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

Farm *FarmUI::getFarm() const {
    return farm;
}




