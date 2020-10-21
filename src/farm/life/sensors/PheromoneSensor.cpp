//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "PheromoneSensor.h"

std::string PheromoneSensor::getName() {
    return "Pheromone Sensor";
}

void PheromoneSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) {
    double tilePheromoneColorValue = accessibleTiles.at(0)->getPheromoneColor();
    double tilePheromoneQuantityValue = accessibleTiles.at(0)->getPheromoneQuantity();

    if (tilePheromoneQuantityValue == 0) {
        value = 0.0;
        return;
    }


    float hueDistances = std::min(abs(tilePheromoneColorValue - color), 1.f - abs(tilePheromoneColorValue - color));

    value = float(pow(2, - (hueDistances * 5)));


    value = std::min(1.0, value * (tilePheromoneQuantityValue / 50.0));

}

void PheromoneSensor::findSelectedChunks() {
    Point entityPoint = this->entity->getPosition();
    Point currentTileCoordinates = entityPoint.getTileCoordinates();
    selectedChunks.clear();
    selectedChunks.emplace_back(currentTileCoordinates.getX(), currentTileCoordinates.getY());
}

PheromoneSensor::PheromoneSensor(Entity *entity) : Sensor(entity), color(0.0) {}

float PheromoneSensor::getColor() const {
    return color;
}

void PheromoneSensor::setColor(float color) {
    PheromoneSensor::color = color;
}

