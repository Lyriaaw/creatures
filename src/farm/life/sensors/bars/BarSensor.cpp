//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include "BarSensor.h"

void BarSensor::findSelectedChunks() {
    Point entityPoint = this->entity->getPosition();
    Point currentSimplePosition = entityPoint.getSimpleCoordinates();


    this->selectedChunks.clear();


    float currentRotation = (rotation  + this->entity->getRotation()) * float(M_PI);

    float sensorX = (cos(currentRotation) * getLength()) + this->entity->getPosition().getX();
    float sensorY = (sin(currentRotation) * getLength()) + this->entity->getPosition().getY();

    Point sensorPosition = {sensorX, sensorY};

    Point sensorSimplePosition = sensorPosition.getSimpleCoordinates();

    int minChunkX = std::min(sensorSimplePosition.getX(), currentSimplePosition.getX()) - 1;
    int maxChunkX = std::max(sensorSimplePosition.getX(), currentSimplePosition.getX()) + 1;

    if (minChunkX < 0) {
        minChunkX = 0;
    }
    if (maxChunkX >= CHUNK_COUNT_WIDTH) {
        maxChunkX = CHUNK_COUNT_WIDTH - 1;
    }


    int minChunkY = std::min(sensorSimplePosition.getY(), currentSimplePosition.getY()) - 1;
    int maxChunkY = std::max(sensorSimplePosition.getY(), currentSimplePosition.getY()) + 1;

    if (minChunkY < 0) {
        minChunkY = 0;
    }
    if (maxChunkY >= CHUNK_COUNT_HEIGHT) {
        maxChunkY = CHUNK_COUNT_HEIGHT - 1;
    }



    // Iterating through the chunks under the sensor end and the creature
    for (int x = minChunkX; x <= maxChunkX; x++) {
        for (int y = minChunkY; y <= maxChunkY; y++) {
            // If the chunk have not already been selected, select it
            bool found(false);
            int index = 0;
            while(!found && index < selectedChunks.size()) {
                if (selectedChunks.at(index).equals(Point(x, y))) {
                    found = true;
                }
                index++;
            };

            if (!found) {
                selectedChunks.emplace_back(x, y);
            }
        }
    }
}


void BarSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) {

    float currentRotation = (rotation + this->entity->getRotation()) * float(M_PI);

    float sensorX = (cos(currentRotation) * (length * this->entity->getSize())) + this->entity->getPosition().getX();
    float sensorY = (sin(currentRotation) * (length * this->entity->getSize())) + this->entity->getPosition().getY();

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

        if (entitiesDistance > (this->length * entity->getSize()) + currentAccessibleEntity->getSize()) {
            continue;
        }

        if (entitiesDistance > currentDistance) {
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

BarSensor::BarSensor(Entity *entity, double rotation, double length) : Sensor(entity), rotation(rotation),
                                                                     length(length) {}

double BarSensor::getRotation() const {
    return rotation;
}

double BarSensor::getLength() const {
    return length * entity->getSize();
}

