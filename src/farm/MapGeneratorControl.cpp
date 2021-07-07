//
// Created by Amalric Lombard de Buffières on 10/27/20.
//

#include "MapGeneratorControl.h"

MapGeneratorControl::MapGeneratorControl(): positionRatio(1.5), zOrdinate(0.8), waterLevel(0.45), layers(4) {
//MapGeneratorControl::MapGeneratorControl(): positionRatio(1.87), zOrdinate(0.8), waterLevel(0.42), layers(1) {

}

int MapGeneratorControl::getSeed() const {
    return seed;
}

void MapGeneratorControl::setSeed(int seed) {
    MapGeneratorControl::seed = seed;
}

float MapGeneratorControl::getPositionRatio() const {
    return positionRatio;
}

void MapGeneratorControl::setPositionRatio(float positionRatio) {
    MapGeneratorControl::positionRatio = positionRatio;
}

float MapGeneratorControl::getZOrdinate() const {
    return zOrdinate;
}

void MapGeneratorControl::setZOrdinate(float zOrdinate) {
    MapGeneratorControl::zOrdinate = zOrdinate;
}


nlohmann::json MapGeneratorControl::asJson() {
    nlohmann::json result = {
        {"seed", seed},
        {"positionRatio", positionRatio},
        {"zOrdinate", zOrdinate},
        {"waterLevel", waterLevel},
        {"layers", layers},
    };

    return result;
}

float MapGeneratorControl::getWaterLevel() const {
    return waterLevel;
}

void MapGeneratorControl::setWaterLevel(float waterLevel) {
    MapGeneratorControl::waterLevel = waterLevel;
}

int MapGeneratorControl::getLayers() const {
    return layers;
}

void MapGeneratorControl::setLayers(int layers) {
    MapGeneratorControl::layers = layers;
}
