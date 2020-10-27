//
// Created by Amalric Lombard de Buffières on 10/27/20.
//

#ifndef CREATURES_MAPGENERATORCONTROL_H
#define CREATURES_MAPGENERATORCONTROL_H
#include <nlohmann/json.hpp>


class MapGeneratorControl {
private:
    int seed;
    float positionRatio;
    float zOrdinate;

    float waterLevel;
    int layers;

public:
    MapGeneratorControl();

    int getSeed() const;

    void setSeed(int seed);

    float getPositionRatio() const;

    void setPositionRatio(float positionRatio);

    float getZOrdinate() const;

    void setZOrdinate(float zOrdinate);

    float getWaterLevel() const;

    void setWaterLevel(float waterLevel);

    int getLayers() const;

    void setLayers(int layers);

    nlohmann::json asJson();
};


#endif //CREATURES_MAPGENERATORCONTROL_H
