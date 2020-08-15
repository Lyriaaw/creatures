//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include "BarSensor.h"

void BarSensor::findSelectedTiles() {
    Point entityPoint = this->entity->getPosition();
    Point currentTilePosition = entityPoint.getTileCoordinates();


    this->selectedTiles.clear();


    float currentRotation = (rotation  + this->entity->getRotation()) * float(M_PI);

    float sensorX = (cos(currentRotation) * length) + this->entity->getPosition().getX();
    float sensorY = (sin(currentRotation) * length) + this->entity->getPosition().getY();

    Point sensorPosition = {sensorX, sensorY};

    Point sensorTilePosition = sensorPosition.getTileCoordinates();

    int minChunkX = std::min(sensorTilePosition.getX(), currentTilePosition.getX()) - 1;
    int maxChunkX = std::max(sensorTilePosition.getX(), currentTilePosition.getX()) + 1;

    if (minChunkX < 0) {
        minChunkX = 0;
    }
    if (maxChunkX >= TILE_COUNT_WIDTH) {
        maxChunkX = TILE_COUNT_WIDTH - 1;
    }


    int minChunkY = std::min(sensorTilePosition.getY(), currentTilePosition.getY()) - 1;
    int maxChunkY = std::max(sensorTilePosition.getY(), currentTilePosition.getY()) + 1;

    if (minChunkY < 0) {
        minChunkY = 0;
    }
    if (maxChunkY >= TILE_COUNT_HEIGHT) {
        maxChunkY = TILE_COUNT_HEIGHT - 1;
    }



    // Iterating through the chunks under the sensor end and the creature
    for (int x = minChunkX; x <= maxChunkX; x++) {
        for (int y = minChunkY; y <= maxChunkY; y++) {
            // If the chunk have not already been selected, select it
            bool found(false);
            int index = 0;
            while(!found && index < selectedTiles.size()) {
                if (selectedTiles.at(index).equals(Point(x, y))) {
                    found = true;
                }
                index++;
            };

            if (!found) {
                selectedTiles.emplace_back(x, y);
            }
        }
    }
}


void BarSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) {

    float currentRotation = (rotation + this->entity->getRotation()) * float(M_PI);

    float sensorX = (cos(currentRotation) * length) + this->entity->getPosition().getX();
    float sensorY = (sin(currentRotation) * length) + this->entity->getPosition().getY();

    float deltaX = this->entity->getPosition().getX() - sensorX;
    float deltaY = this->entity->getPosition().getY() - sensorY;

    float m = 0.f;
    float p = 0.f;

    if (deltaX != 0 && deltaY != 0) {
        m = (this->entity->getPosition().getY() - sensorY) / (this->entity->getPosition().getX() - sensorX);
        p = sensorY - (sensorX * m);
    }

    if (deltaY == 0) {
        m = 1;
        p = sensorY;
    }

    getSensorValueFromSensorEquation(sensorX, sensorY, m, p, accessibleEntities);

}


void BarSensor::getSensorValueFromSensorEquation(float sensorX, float sensorY, double m, double p, std::vector<Entity *> accessibleEntities) {
    double currentDistance(FARM_WIDTH);

    Entity *closestEntity = nullptr;


    for (int jt = 0; jt < accessibleEntities.size(); jt++) {
        Entity *currentAccessibleEntity = accessibleEntities.at(jt);

        if (currentAccessibleEntity->getId() == this->entity->getId()) {
            continue;
        }

        double entitiesDistance = sqrt(
                pow(this->entity->getPosition().getX() - currentAccessibleEntity->getPosition().getX(), 2) +
                pow(this->entity->getPosition().getY() - currentAccessibleEntity->getPosition().getY(), 2));

        if (entitiesDistance > this->length + currentAccessibleEntity->getSize()) {
            continue;
        }



        // Check if the entity is between the two ends of the sensor
        // Add the size of the entity for margin purposes
        if (
                !(std::min(this->entity->getPosition().getX(), sensorX) - currentAccessibleEntity->getSize() <=
                  currentAccessibleEntity->getPosition().getX() &&
                  std::max(this->entity->getPosition().getX(), sensorX) + currentAccessibleEntity->getSize() >=
                  currentAccessibleEntity->getPosition().getX() &&
                  std::min(this->entity->getPosition().getY(), sensorY) - currentAccessibleEntity->getSize() <=
                  currentAccessibleEntity->getPosition().getY() &&
                  std::max(this->entity->getPosition().getY(), sensorY) + currentAccessibleEntity->getSize() >=
                  currentAccessibleEntity->getPosition().getY())
                ) {
            continue;
        }

        float numerator = abs(
                -(m * currentAccessibleEntity->getPosition().getX()) + currentAccessibleEntity->getPosition().getY() -
                p);
        float denominator = sqrt(pow(m, 2) + 1);

        float distanceToSensor = numerator / denominator;


        if (abs(distanceToSensor) > currentAccessibleEntity->getSize()) {
            continue;
        }

        float closestX = ((currentAccessibleEntity->getPosition().getX() + m * currentAccessibleEntity->getPosition().getY()) - (m * p)) / float(pow(m, 2) + 1);
        float closestY = (m * (currentAccessibleEntity->getPosition().getX() + m * currentAccessibleEntity->getPosition().getY()) + p) / float(pow(m, 2) + 1);
        float preciseEntitiesDistance = sqrt( pow(closestX - entity->getPosition().getX(), 2) + pow(closestY - entity->getPosition().getY(), 2));


        float sensorCurrentDistanceValue = preciseEntitiesDistance;

        if (currentDistance <= sensorCurrentDistanceValue) {
            continue;
        }
        currentDistance = sensorCurrentDistanceValue;
        closestEntity = accessibleEntities.at(jt);

        this->processSensorValue(currentDistance, closestEntity);
    }

    if (closestEntity == nullptr) {
        value = 0.0;
    }
}

BarSensor::BarSensor(Entity *entity, double rotation, double lengthRatio) : Sensor(entity), rotation(rotation),
                                                                            lengthRatio(lengthRatio) {}

double BarSensor::getRotation() const {
    return rotation;
}

double BarSensor::getLength() const {
    return length;
}

double BarSensor::getLengthRatio() const {
    return lengthRatio;
}

void BarSensor::setLength(double length) {
    BarSensor::length = length;
}

void BarSensor::setLengthRatio(double lengthRatio) {
    BarSensor::lengthRatio = lengthRatio;
}

