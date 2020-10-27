//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAP_H
#define CREATURES_MAP_H


#include <vector>
#include "../World.h"
#include "Tile.h"
#include "FarmControl.h"
#include "statistics/DataAnalyser.h"
#include "MapGeneratorControl.h"
#include <shared_mutex>
#include <iostream>
#include <nlohmann/json.hpp>


class Map {
private:
    FarmControl * farmControl;

    std::vector<std::vector<Tile *>> tiles;

    double speedCorrectionRatio;
    int tick;

    std::function<void(std::vector<Entity *> entities)> recordAddedEntitiesToFarm;

    DataAnalyser dataAnalyser;

    std::function<void()> triggerUpdate;

    MapGeneratorControl * mapGeneratorControl;

public:



    Map(FarmControl * farmControl);

    Map();

    void initRandomMap();
    void generateRandomTerrain(bool renew);
    void processClimate();
    void vegetalisation();

    void removeEnergyFromGround(double amount);

    Tile * getTileAt(int tileX, int tileY);

    void setTiles(const std::vector<std::vector<Tile *>> &tiles);


    void setRecordAddedEntitiesToFarm(const std::function<void(std::vector<Entity *>)> &recordAddedEntitiesToFarm);

    int getTick() const;
    void setTick(int tick);

    void setSpeedCorrectionRatio(double speedCorrectionRatio);

    void handleThread();

    const DataAnalyser &getDataAnalyser() const;

    void setTriggerUpdate(const std::function<void()> &triggerUpdate);

    MapGeneratorControl *getMapGeneratorControl() const;

    nlohmann::basic_json<> asJson();
};


#endif //CREATURES_MAP_H
