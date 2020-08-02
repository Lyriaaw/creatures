//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include <iostream>
#include "Creature.h"
#include "../World.h"

Creature::Creature(Point position, float size): Entity(position, size) {
    this->color = (rand() % 1000) / 1000.f;
    this->brightness = 0.5f;
    this->rotation = (rand() % 628) / 100.f;



    int sensorCount = rand() % 2 + 3;
//    int sensorCount = 1;
    for (int it = 0; it < sensorCount; it++) {
        float currentSensorLength = (((rand() % 300) / 100.f) + 2) * this->size;


        float randomRotation = ((rand() % 200) / 100.f) - 1.f ;

        float currentSensorRotation =  randomRotation * M_PI;
        sensorRotations.push_back(currentSensorRotation);
        sensorLengths.push_back(currentSensorLength);
        sensorDistances.push_back(0);
    }
}

void Creature::move() {
    float addedSpeed = (float(rand() % 21) - 10.f) / 10.f;
    float addedRotation = (float(rand() % 21) - 10.f) / 100.f;

    this->rotation += addedRotation;
    this->speed += addedSpeed;

    if (this->speed > 5) {
        this->speed = 5;
    }
    if (this->speed < -2.5f) {
        this->speed = -2.5f;
    }


    float nextX = this->position.getX() + (this->speed * cos(this->rotation));
    float nextY = this->position.getY() + (this->speed * sin(this->rotation));



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

}

void Creature::findSelectedChunks() {
    std::vector<Point> sensorPositions;
    std::vector<Point> sensorSimplePositions;
    Point currentSimplePosition = this->position.getSimpleCoordinates();

    this->selectedChunks.clear();


    int sensorCount = getSensorCount();
    for (int it = 0; it < sensorCount; it++) {
        float currentLength = getSensorLength(it);
        float currentRotation = getSensorRotation(it) + this->rotation;

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

void Creature::getSensorCoordinates(std::vector<Entity *> accessibleEntities, Creature * selectedCreature) {
    this->accessibleEntities = accessibleEntities;

    Point currentCreaturePosition = this->position.getSimpleCoordinates();

    int sensorCount = getSensorCount();
    for (int it = 0; it < sensorCount; it++) {
        bool found(false);

        float currentLength = getSensorLength(it);
        float currentRotation = getSensorRotation(it) + this->rotation;

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


        for (int jt = 0; jt < accessibleEntities.size(); jt++) {
            Entity * currentAccessibleEntity = accessibleEntities.at(jt);

            if (currentAccessibleEntity == this) {
                continue;
            }

//            if ((this->position.getX() - currentAccessibleEntity->getPosition().getX()) * (this->position.getX() - sensorX) < 0 ||
//                    (this->position.getY() - currentAccessibleEntity->getPosition().getY()) * (this->position.getY() - sensorY) < 0) {
//
//                std::cout << "Not in the correct direction" << std::endl;
//                continue;
//            }

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


            if (abs(distanceToSensor) <= currentAccessibleEntity->getSize()) {
                found = true;
            }
        }

        if (found) {
            this->sensorDistances.insert(this->sensorDistances.begin() + it, 1);
        } else {
            this->sensorDistances.insert(this->sensorDistances.begin() + it, 0);
        }

     }



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


const std::vector<Point> &Creature::getSelectedChunks() const {
    return selectedChunks;
}

const std::vector<Entity *> &Creature::getAccessibleEntities() const {
    return accessibleEntities;
}
