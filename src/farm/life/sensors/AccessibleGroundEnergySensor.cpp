//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#include "AccessibleGroundEnergySensor.h"

std::string AccessibleGroundEnergySensor::getName() {
    return "ACCESSIBLE_GROUND_ENERGY";
}

void AccessibleGroundEnergySensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) {
    double totalGroundEnergy = 0.0;


    for (int it = 0; it < selectedChunks.size(); it++) {

        for (int jt = 0; jt < availableTiles.size(); jt++) {
            if (!selectedChunks.at(it).equals(availableTiles.at(jt)->getPosition())) {
                continue;
            }

            totalGroundEnergy += availableTiles.at(jt)->getGround();
        }
    }

    float biggestEntity = std::max(double(this->entity->getMass()), totalGroundEnergy);
    float smallestEntity = std::min(double(this->entity->getMass()), totalGroundEnergy);

    float sizeRatio = 1 - (smallestEntity / biggestEntity);
    if (this->entity->getMass() > totalGroundEnergy) {
        sizeRatio *= -1;
    }
    value = (0.5f + (sizeRatio / 2.f));
}

void AccessibleGroundEnergySensor::findSelectedChunks() {
    this->selectedChunks.clear();


    int chunkReach = this->entity->getSize() / 5;

    Point entityPoint = this->entity->getPosition();
    Point tilePoint = entityPoint.getTileCoordinates();

    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            if (currentTileX < 0)
                currentTileX = 0;
            if (currentTileY < 0)
                currentTileY = 0;
            if (currentTileX >= TILE_COUNT_WIDTH)
                currentTileX = TILE_COUNT_WIDTH - 1;
            if (currentTileY >= TILE_COUNT_HEIGHT)
                currentTileY = TILE_COUNT_HEIGHT - 1;

            selectedChunks.emplace_back(Point(currentTileX, currentTileY));

        }
    }


}

AccessibleGroundEnergySensor::AccessibleGroundEnergySensor(Entity *entity) : Sensor(entity) {}
