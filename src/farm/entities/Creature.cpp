//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include <iostream>
#include "Creature.h"


Creature::Creature(Point position): Entity(position) {
    this->brightness = 0.5f;
//    this->rotation = ((rand() % 200) / 100.f) - 1.f;
    this->rotation = 0.f;

    mouthValue = 0;
    genitalsValue = 0;
}

int Creature::addVisionSensorSensor(float length, float rotation, float color) {
    sensorLengths.push_back(length);
    sensorRotations.push_back(rotation);
    sensorColors.push_back(color);

    sensorDistances.push_back(0);
    sensorBrightness.push_back(0);

    return getSensorCount() - 1;
}

double Creature::move() {

    float actualSpeed = this->speed * this->size;

    if (actualSpeed < -1 * (this->size / 2.f)) {
        actualSpeed = -1 * (this->size / 2.f);
    }


    float nextX = this->position.getX() + (actualSpeed * cos((this->rotation * M_PI)));
    float nextY = this->position.getY() + (actualSpeed * sin((this->rotation * M_PI)));



    if (nextX < 0) {
        nextX = 0;
    }
    if (int(nextX) >= FARM_WIDTH) {
        nextX = FARM_WIDTH - 1;
    }
    if (nextY < 0) {
        nextY = 0;
    }
    if (int(nextY) >= FARM_HEIGHT) {
        nextY = FARM_HEIGHT - 1;
    }

    this->position.setX(nextX);
    this->position.setY(nextY);



    double speedEnergy = abs(actualSpeed);
    double mouthEnergy = (mouthValue * this->size);
    double genitalsEnergy = abs(genitalsValue * this->size);
    double sensorEnergy = getSensorCount();
    double biasEnergy = 10;

    double usedEnergy =  mouthEnergy + genitalsEnergy + sensorEnergy + speedEnergy + biasEnergy;

    if (this->energy - usedEnergy < 0) {
        usedEnergy = this->energy;
    }

    if (usedEnergy < 0) {
        std::cout << "Returned negative amount of energy: " << usedEnergy << std::endl;

    }

    this->energy -= usedEnergy;
    return usedEnergy;

}

void Creature::findSelectedChunks() {
    std::vector<Point> sensorPositions;
    std::vector<Point> sensorSimplePositions;
    Point currentSimplePosition = this->position.getSimpleCoordinates();

    this->selectedChunks.clear();


    int sensorCount = getSensorCount();
    for (int it = 0; it < sensorCount; it++) {
        float currentLength = getSensorLength(it);
        float currentRotation = (getSensorRotation(it)  + this->rotation) * float(M_PI);

        float sensorX = (cos(currentRotation) * currentLength) + this->position.getX();
        float sensorY = (sin(currentRotation) * currentLength) + this->position.getY();

        Point sensorPoint = {sensorX, sensorY};
        sensorPositions.push_back(sensorPoint);

        Point sensorSimplePoint = sensorPoint.getSimpleCoordinates();
        sensorSimplePositions.push_back(sensorPoint);

        int minChunkX = std::min(sensorSimplePoint.getX(), currentSimplePosition.getX()) - 1;
        int maxChunkX = std::max(sensorSimplePoint.getX(), currentSimplePosition.getX()) + 1;

        if (minChunkX < 0) {
            minChunkX = 0;
        }
        if (maxChunkX >= CHUNK_COUNT_WIDTH) {
            maxChunkX = CHUNK_COUNT_WIDTH - 1;
        }


        int minChunkY = std::min(sensorSimplePoint.getY(), currentSimplePosition.getY()) - 1;
        int maxChunkY = std::max(sensorSimplePoint.getY(), currentSimplePosition.getY()) + 1;

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
}


void Creature::processSensorsValues(std::vector<Entity *> accessibleEntities) {
    int sensorCount = getSensorCount();
    for (int it = 0; it < sensorCount; it++) {

        float currentLength = getSensorLength(it);
        float currentRotation = (sensorRotations.at(it) + this->rotation) * float(M_PI);

        float sensorX = (cos(currentRotation) * currentLength) + this->position.getX();
        float sensorY = (sin(currentRotation) * currentLength) + this->position.getY();

        float deltaX = this->position.getX() - sensorX;
        float deltaY = this->position.getY() - sensorY;

        float m = 0.f;
        float p = 0.f;

        if (deltaX != 0 && deltaY != 0) {
            m = (this->position.getY() - sensorY) / (this->position.getX() - sensorX);
            p = sensorY - (sensorX * m);
        }

        if (deltaY == 0) {
            m = 1;
            p = sensorY;
        }

        getSensorValueFromSensorEquation(it, sensorX, sensorY, m, p, accessibleEntities);

    }



}

void Creature::getSensorValueFromSensorEquation(int sensorIndex, float sensorX, float sensorY, float m, float p, std::vector<Entity *> accessibleEntities) {
    float sensorDistanceValue(0.f);
    float sensorDistanceBrightness(0.f);
    float sensorSizeValue(0.f);

    for (int jt = 0; jt < accessibleEntities.size(); jt++) {
        Entity * currentAccessibleEntity = accessibleEntities.at(jt);

        if (currentAccessibleEntity == this) {
            continue;
        }

        float entitiesDistance = sqrt(pow(this->position.getX() - currentAccessibleEntity->getPosition().getX(), 2) + pow(this->position.getY() - currentAccessibleEntity->getPosition().getY(), 2));

        if (entitiesDistance > this->sensorLengths.at(sensorIndex) + currentAccessibleEntity->getSize()) {
            continue;
        }

        // Check if the entity is between the two ends of the sensor
        // Add the size of the entity for margin purposes
        if (
                !(std::min(this->position.getX(), sensorX) - currentAccessibleEntity->getSize() <= currentAccessibleEntity->getPosition().getX() &&
                  std::max(this->position.getX(), sensorX) + currentAccessibleEntity->getSize() >= currentAccessibleEntity->getPosition().getX() &&
                  std::min(this->position.getY(), sensorY) - currentAccessibleEntity->getSize() <= currentAccessibleEntity->getPosition().getY() &&
                  std::max(this->position.getY(), sensorY) + currentAccessibleEntity->getSize() >= currentAccessibleEntity->getPosition().getY())
                )
        {
            continue;
        }

        float numerator = abs(-(m * currentAccessibleEntity->getPosition().getX()) + currentAccessibleEntity->getPosition().getY() - p);
        float denominator = sqrt(pow(m, 2) + 1);

        float distanceToSensor = numerator / denominator;


        if (abs(distanceToSensor) > currentAccessibleEntity->getSize()) {
            continue;
        }


        float closestX = ((currentAccessibleEntity->getPosition().getX() + m * currentAccessibleEntity->getPosition().getY()) - (m * p)) / float(pow(m, 2) + 1);
        float closestY = (m * (currentAccessibleEntity->getPosition().getX() + m * currentAccessibleEntity->getPosition().getY()) + p) / float(pow(m, 2) + 1);
        float preciseEntitiesDistance = sqrt(pow(closestX - position.getX(), 2) + pow(closestY - position.getY(), 2));

        float currentSensorLength = sensorLengths[sensorIndex];

        float sensorCurrentDistanceValue = 1.f - (preciseEntitiesDistance / currentSensorLength);

        if (sensorCurrentDistanceValue <= sensorDistanceValue) {
            continue;
        }

        sensorDistanceValue = sensorCurrentDistanceValue;

        float hueDistances = std::min(abs(currentAccessibleEntity->getColor() - this->sensorColors.at(sensorIndex)), 1.f - abs(currentAccessibleEntity->getColor() - this->sensorColors.at(sensorIndex)));
        sensorDistanceBrightness = float(pow(2, - (hueDistances * 5)));

        float biggestEntity = std::max(this->size, currentAccessibleEntity->getSize());
        float smallestEntity = std::min(this->size, currentAccessibleEntity->getSize());

        float sizeRatio = 1 - (smallestEntity / biggestEntity);
        if (this->size > currentAccessibleEntity->getSize()) {
            sizeRatio *= -1;
        }
        sensorSizeValue = (0.5f + (sizeRatio / 2.f));
//        sensorSizeValue = sizeRatio;
    }

    this->sensorDistances.insert(this->sensorDistances.begin() + sensorIndex, sensorDistanceValue);
    this->sensorBrightness.insert(this->sensorBrightness.begin() + sensorIndex, sensorDistanceBrightness);
    this->sensorSizes.insert(this->sensorSizes.begin() + sensorIndex, sensorSizeValue);
}


std::vector<ActionDTO> Creature::executeAction(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    std::vector<ActionDTO> mouthAction = getMouthAction(accessibleEntities);
    std::vector<ActionDTO> genitalsActions = getGenitalsAction(accessibleEntities);

    actions.insert(actions.begin(), mouthAction.begin(), mouthAction.end());
    actions.insert(actions.begin(), genitalsActions.begin(), genitalsActions.end());


    return actions;
}

std::vector<ActionDTO> Creature::getMouthAction(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    if (mouthValue < 0.5f) {
        return actions;
    }

    float mouthTotalRotation = (float(mouthRotation) + rotation) * float(M_PI);

    float mouthSize = size / 3.f;
    float mouthX = (cos(mouthTotalRotation) * size) + position.getX();
    float mouthY = (sin(mouthTotalRotation) * size) + position.getY();

    Entity * closestEntity = nullptr;
    float smallestDistance = FARM_WIDTH;

    for (int it = 0; it < accessibleEntities.size(); it++) {
        float distanceX = abs(accessibleEntities.at(it)->getPosition().getX() - mouthX);
        float distanceY = abs(accessibleEntities.at(it)->getPosition().getY() - mouthY);

        float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

        if (distance <= mouthSize) {
            if (distance < smallestDistance) {
                smallestDistance = distance;
                closestEntity = accessibleEntities.at(it);
            }
        }
    }

    if (closestEntity == nullptr) {
        return actions;
    }

    if (closestEntity->getSize() <= mouthSize) {
        ActionDTO action = ActionDTO(this->id, closestEntity->getId(), "EAT");
        actions.emplace_back(action);
    }
    return actions;
}

std::vector<ActionDTO> Creature::getGenitalsAction(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    if (genitalsValue < 0.5f) {
        return actions;
    }

    float genitalsTotalRotation = (float(genitalsRotation) + rotation) * float(M_PI);

    float genitalsSize = size / 4.f;
    float genitalsX = (cos(genitalsTotalRotation) * size) + position.getX();
    float genitalsY = (sin(genitalsTotalRotation) * size) + position.getY();

    Entity * closestEntity = nullptr;
    float smallestDistance = FARM_WIDTH;

    for (int it = 0; it < accessibleEntities.size(); it++) {
        float distanceX = abs(accessibleEntities.at(it)->getPosition().getX() - genitalsX);
        float distanceY = abs(accessibleEntities.at(it)->getPosition().getY() - genitalsY);

        float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

        if (distance <= genitalsSize) {
            if (distance < smallestDistance) {
                smallestDistance = distance;
                closestEntity = accessibleEntities.at(it);
            }
        }
    }

    if (closestEntity == nullptr) {
        return actions;
    }

    if (smallestDistance <= closestEntity->getSize()) {
        ActionDTO action = ActionDTO(this->id, closestEntity->getId(), "MATE");
        actions.emplace_back(action);
    }
    return actions;
}



void Creature::addSpeed(float speedToAdd) {
    this->speed += speedToAdd;
}

void Creature::addRotation(float rotationToAdd) {
    this->rotation += rotationToAdd;
}


int Creature::getSensorCount() {
    return sensorRotations.size();
}

float Creature::getSensorRotation(int index) {
    return sensorRotations.at(index);
}
float Creature::getSensorLength(int index) {
    return sensorLengths.at(index);
}
float Creature::getSensorDistance(int index) {
    return sensorDistances.at(index);
}
float Creature::getSensorBrightness(int index) {
    return sensorBrightness.at(index);
}
float Creature::getSensorSizes(int index) {
    return sensorSizes.at(index);
}

float Creature::getSensorColor(int index) {
    return sensorColors.at(index);
}



const std::vector<Point> &Creature::getSelectedChunks() const {
    return selectedChunks;
}

float Creature::getMouthRotation() const {
    return mouthRotation;
}

void Creature::setMouthRotation(float mouthRotation) {
    Creature::mouthRotation = mouthRotation;
}

float Creature::getGenitalsRotation() const {
    return genitalsRotation;
}

void Creature::setGenitalsRotation(float genitalsRotation) {
    Creature::genitalsRotation = genitalsRotation;
}

float Creature::getMouthValue() const {
    return mouthValue;
}

void Creature::setMouthValue(float mouthValue) {
    Creature::mouthValue = mouthValue;
}

float Creature::getGenitalsValue() const {
    return genitalsValue;
}

void Creature::setGenitalsValue(float genitalsValue) {
    Creature::genitalsValue = genitalsValue;
}



