//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include <iostream>
#include <sstream>

#include "FarmUI.h"
#include "Camera.h"
#include "../colors/RGBColor.h"
#include "../elements/GlobalFont.h"
#include "../minimaps/ActionsMinimap.h"
#include "../minimaps/CreatureTileCountMinimap.h"
#include "../minimaps/FoodTileCountMinimap.h"
#include "../minimaps/PheromoneMinimap.h"

FarmUI::FarmUI(Farm *farm): farm(farm), hoveredTile(Point(-1, -1)), selectedLife(nullptr) {
    loadTexts();
    loadFarm();

    currentMinimap = new WorldMinimap();
    pheromoneMinimap = new PheromoneMinimap();
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
    pheromoneTilesVertexArray = sf::VertexArray(sf::Quads, TILE_COUNT_WIDTH * TILE_COUNT_HEIGHT * 4);
}

void FarmUI::loadLifes() {
    for (int it = 0; it < farm->getLifes().size(); it++) {
        Life * currentLife = farm->getLifes().at(it);

        LifeUI * ui = new LifeUI(currentLife, GlobalFont::MainFont);
        lifeUIs.emplace_back(ui);
    }
}

void FarmUI::loadEntities() {
    std::vector<Entity *> entities = farm->getEntities();
    for (int it = 0; it < entities.size(); it++) {
        Entity * currentEntity = entities.at(it);

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


void FarmUI::clearDeletedLifes() {
    std::vector<LifeUI *> newLifeUis;
    for (int it = 0; it < lifeUIs.size(); it++) {
        LifeUI * currentLifeUi = lifeUIs.at(it);

        if (currentLifeUi->getLife()->isAlive()) {
            newLifeUis.emplace_back(currentLifeUi);
        }
    }
    lifeUIs = newLifeUis;
}

void FarmUI::clearDeletedEntities() {
    std::vector<EntityUI *> newEntityUis;
    for (int it = 0; it < entityUIs.size(); it++) {
        EntityUI * currentEntityUI = entityUIs.at(it);

        if (currentEntityUI->getEntity()->isExists()) {
            newEntityUis.emplace_back(currentEntityUI);
        }
    }
    entityUIs = newEntityUis;
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
    float currentPhColor = farm->getMap()->getTileAt(currentHoveredTile.getX(), currentHoveredTile.getY())->getPheromoneColor();
    float correntPhQuantity = farm->getMap()->getTileAt(currentHoveredTile.getX(), currentHoveredTile.getY())->getPheromoneQuantity();
    tileInfo = tileInfo + "Height: " + std::to_string(currentHeight) + "\n";
    tileInfo = tileInfo + "Heat: " + std::to_string(currentHeat) + "\n";
    tileInfo = tileInfo + "Ground: " + std::to_string(currentGround) + "\n";
    tileInfo = tileInfo + "Ph Color: " + std::to_string(currentPhColor) + "\n";
    tileInfo = tileInfo + "Ph Quant: " + std::to_string(correntPhQuantity) + "\n";



    hoveredTileInfos.setString(tileInfo);
}



void FarmUI::update() {
    // TODO OPTI Clear to_delete

    clearDeletedEntities();

    clearDeletedLifes();

    addLifes(farm->getAndClearLifesToAdd());

    addEntities(farm->getAndClearEntitiesToAdd());

    generateTileInfoText();
}

sf::Color FarmUI::getColorForTile(int x, int y, Camera * camera, Map *map) {
    sf::Color color = currentMinimap->getColorAt(x, y);

    if (selectedLife == nullptr) {
        return color;
    }


    for (auto const& currentAccessibleTile : selectedLife->getCurrentAccessibleTiles()) {
        if (currentAccessibleTile->getCoordinates().getX() == x && currentAccessibleTile->getCoordinates().getY() == y) {
            return color;
        }
    }

    return {color.r, color.g, color.b, 200};

}

void FarmUI::setPositions(Camera *camera) {
//     std::cout << "Setting positions " << camera->getZoom() << std::endl;

    if (camera == nullptr) {
        return;
    }

    Map *map = farm->getMap();
    currentMinimap->generateValues(farm);
    pheromoneMinimap->generateValues(farm);

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


                pheromoneTilesVertexArray[vertexArrayIndex + 0].position = sf::Vector2f(-10, -10);
                pheromoneTilesVertexArray[vertexArrayIndex + 1].position = sf::Vector2f(-10, -10);
                pheromoneTilesVertexArray[vertexArrayIndex + 2].position = sf::Vector2f(-10, -10);
                pheromoneTilesVertexArray[vertexArrayIndex + 3].position = sf::Vector2f(-10, -10);


                continue;
            }


            int gridXRatio = 0;
            int gridYRatio = 0;

            if (camera->isShowGrid()) {
                gridXRatio = 1;
                gridYRatio = 1;

                if ((it + 1) % TILE_PER_CHUNK == 0) {
                    gridXRatio = 2;
                }
                if ((jt + 1) % TILE_PER_CHUNK == 0) {
                    gridYRatio = 2;
                }


            }



            int vertexArrayIndex = ((it * TILE_COUNT_HEIGHT) + jt) * 4;
            Point point = camera->getScreenCoordinates({float(it) * TILE_SIZE, float(jt) * TILE_SIZE});
            tilesVertexArray[vertexArrayIndex + 0].position = sf::Vector2f(
                    std::max(double(point.getX()), double(camera->getTopLeft().getX())),
                    std::max(double(point.getY()), double(camera->getTopLeft().getY())));

            pheromoneTilesVertexArray[vertexArrayIndex + 0].position = sf::Vector2f(
                    std::max(double(point.getX()), double(camera->getTopLeft().getX())),
                    std::max(double(point.getY()), double(camera->getTopLeft().getY())));




            double currentX = point.getX() + (TILE_SIZE * camera->getZoom()) - gridXRatio;
            double currentY = point.getY() + (TILE_SIZE * camera->getZoom()) - gridYRatio;
            tilesVertexArray[vertexArrayIndex + 1].position = sf::Vector2f(
                    std::min(double(currentX), double(camera->getTopLeft().getX()) + double(camera->getWidth())),
                    std::max(double(point.getY()), double(camera->getTopLeft().getY())));

            pheromoneTilesVertexArray[vertexArrayIndex + 1].position = sf::Vector2f(
                    std::min(double(currentX), double(camera->getTopLeft().getX()) + double(camera->getWidth())),
                    std::max(double(point.getY()), double(camera->getTopLeft().getY())));




            tilesVertexArray[vertexArrayIndex + 2].position = sf::Vector2f(
                    std::min(double(currentX), double(camera->getTopLeft().getX()) + double(camera->getWidth())),
                    std::min(double(currentY), double(camera->getTopLeft().getY()) + double(camera->getHeight())));

            pheromoneTilesVertexArray[vertexArrayIndex + 2].position = sf::Vector2f(
                    std::min(double(currentX), double(camera->getTopLeft().getX()) + double(camera->getWidth())),
                    std::min(double(currentY), double(camera->getTopLeft().getY()) + double(camera->getHeight())));




            tilesVertexArray[vertexArrayIndex + 3].position = sf::Vector2f(
                    std::max(double(point.getX()), double(camera->getTopLeft().getX())),
                    std::min(double(currentY), double(camera->getTopLeft().getY()) + double(camera->getHeight())));

            pheromoneTilesVertexArray[vertexArrayIndex + 3].position = sf::Vector2f(
                    std::max(double(point.getX()), double(camera->getTopLeft().getX())),
                    std::min(double(currentY), double(camera->getTopLeft().getY()) + double(camera->getHeight())));






            sf::Color tileColor = getColorForTile(it, jt, camera, map);
            tilesVertexArray[vertexArrayIndex + 0].color = tileColor;
            tilesVertexArray[vertexArrayIndex + 1].color = tileColor;
            tilesVertexArray[vertexArrayIndex + 2].color = tileColor;
            tilesVertexArray[vertexArrayIndex + 3].color = tileColor;

            sf::Color pheromoneColor = pheromoneMinimap->getColorAt(it, jt);
            pheromoneTilesVertexArray[vertexArrayIndex + 0].color = pheromoneColor;
            pheromoneTilesVertexArray[vertexArrayIndex + 1].color = pheromoneColor;
            pheromoneTilesVertexArray[vertexArrayIndex + 2].color = pheromoneColor;
            pheromoneTilesVertexArray[vertexArrayIndex + 3].color = pheromoneColor;




        }
    }
}

void FarmUI::draw(sf::RenderWindow *window, Camera *camera, Life * selectedEntity) {
    Entity * selected = nullptr;

    if (selectedEntity != nullptr) {
        selected = selectedEntity->getEntity();
    }


    std::vector<EntityUI *> currentEntities = entityUIs;

    std::vector<LifeUI *> currentLifes = lifeUIs;
    setPositions(camera);

    window->draw(tilesVertexArray);

    if (currentMinimap->getName() == "World") {
        window->draw(pheromoneTilesVertexArray);
    }

    for (int it = 0; it < currentEntities.size(); it++) {
        currentEntities.at(it)->draw(window, camera, selectedEntity);
    }

    for (int it = 0; it < currentLifes.size(); it++) {
        currentLifes.at(it)->draw(window, camera, selected);
    }

    Point worldCoordinates = Point(hoveredTile.getX() * TILE_SIZE, hoveredTile.getY() * TILE_SIZE);

    if (camera->shouldDisplayPoint(camera->getScreenCoordinates(worldCoordinates)) && camera->isShowGrid()) {
        window->draw(hoveredTileInfos);
    }


}





void FarmUI::mouseMoved(Point worldPosition, Camera * camera) {
    hoveredTile = worldPosition.getTileCoordinates();

    Point topLeftChunk = Point(hoveredTile.getX() * TILE_SIZE, hoveredTile.getY() * TILE_SIZE);
    Point screenTopLeft = camera->getScreenCoordinates(topLeftChunk);


    hoveredTileInfos.setPosition(screenTopLeft.getX(), screenTopLeft.getY());
    hoveredTileInfos.setFillColor(sf::Color(100, 0, 0));
    hoveredTileInfos.setCharacterSize(5 * camera->getZoom());
    generateTileInfoText();
}

Farm *FarmUI::getFarm() const {
    return farm;
}

Life *FarmUI::getSelectedLife() const {
    return selectedLife;
}

Entity *FarmUI::getSelectedEntity() const {
    return selectedEntity;
}

void FarmUI::setSelectedLifeChanged(const std::function<void()> &selectedLifeChanged) {
    FarmUI::selectedLifeChanged = selectedLifeChanged;
}

void FarmUI::processClick(float screenX, float screenY, Camera * camera) {
    if (screenX < camera->getTopLeft().getX() || screenY < camera->getTopLeft().getY()) {
        return;
    }

    if (screenX > camera->getTopLeft().getX() + camera->getWidth() || screenY > camera->getTopLeft().getY() + camera->getHeight()) {
        return;
    }



    Point worldCoordinates = camera->getWorldCoordinates({screenX, screenY});
    if (!(worldCoordinates.getX() > 0 && worldCoordinates.getY() > 0 && worldCoordinates.getX() < FARM_WIDTH && worldCoordinates.getY() < FARM_HEIGHT)) {
        return;
    }


    bool found = false;
    std::vector<Life *> currentLifes = farm->getLifes();
    for (int it = 0; it < currentLifes.size(); it++) {
        Life * connector = currentLifes.at(it);

        double deltaX = abs(worldCoordinates.getX() - connector->getEntity()->getPosition().getX());
        double deltaY = abs(worldCoordinates.getY() - connector->getEntity()->getPosition().getY());

        if (deltaX < connector->getEntity()->getSize() && deltaY < connector->getEntity()->getSize()) {
            selectedLife = currentLifes.at(it);

            std::vector<Evolution *>  genome = farm->getNursery()->getEvolutionLibrary().getGenomeFor(selectedLife->getEntity()->getId());
            std::vector<Neuron *> neurons = selectedLife->getBrain()->getNeurons();

            found = true;
            selectedLifeChanged();
        }
    }


    std::vector<Entity *> currentEntities = farm->getEntities();
    for (int it = 0; it < currentEntities.size(); it++) {
        Entity * entity = currentEntities.at(it);

        double deltaX = abs(worldCoordinates.getX() - entity->getPosition().getX());
        double deltaY = abs(worldCoordinates.getY() - entity->getPosition().getY());

        if (deltaX < entity->getSize() && deltaY < entity->getSize()) {
            selectedEntity = entity;

            found = true;
        }
    }

//        for (int it = 0; it < farm->getFoods().size(); it++) {
//            Entity * entity = farm->getFoods().at(it);
//
//            double deltaX = abs(worldCoordinates.getX() - entity->getPosition().getX());
//            double deltaY = abs(worldCoordinates.getY() - entity->getPosition().getY());
//
//            if (deltaX < entity->getSize() && deltaY < entity->getSize()) {
//                selectedEntity = entity;
//                selectedLife = nullptr;
//                found = true;
//            }
//        }

    if (!found) {
        selectedEntity = nullptr;
        selectedLife = nullptr;
        selectedLifeChanged();
    }
}

void FarmUI::selectedCreatureChange(std::string type) {
    std::vector<Life *> currentLifes = farm->getLifes();

    if (type == "RANDOM") {
        int randomCreatureIndex = rand() % currentLifes.size();
        Life * life = currentLifes.at(randomCreatureIndex);
        selectedLife = life;
    }

    if (type == "DISMISS") {
        selectedLife = nullptr;
    }
    if (type == "BEST") {
        selectedLife = farm->getScoreSortedCreatures().at(0);
    }




    selectedLifeChanged();
}

void FarmUI::setCurrentMinimap(Minimap *currentMinimap) {
    FarmUI::currentMinimap = currentMinimap;
}

const std::vector<LifeUI *> &FarmUI::getLifeUIs() const {
    return lifeUIs;
}

const std::vector<EntityUI *> &FarmUI::getEntityUIs() const {
    return entityUIs;
}

