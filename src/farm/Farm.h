//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARM_H
#define CREATURES_FARM_H




#include "entities/Entity.h"
#include "Map.h"
#include "entities/Creature.h"
#include "entities/Food.h"
#include "CreatureNursery.h"
#include "statistics/DataAnalyser.h"
#include "life/Life.h"
#include "Chunk.h"

class Farm {
private:
    std::vector<std::vector<Chunk *>> chunks;


    std::vector<Life *> sorted;

    std::vector<Life *> lifes;
    std::vector<Entity *> entities;

//    Map * map;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;
    std::vector<std::vector<std::vector<Life *>>> chunkLifeGrid;
    std::vector<std::vector<std::vector<Entity *>>> chunkEntityGrid;
    std::vector<std::vector<std::vector<ActionDTO>>> actionsGrid;

    CreatureNursery * nursery;

    std::vector<ActionDTO> actions;

    double availableEnergy;


    std::vector<Life *> lifesAdded;
    std::vector<Life *> lifesToDelete;

    std::vector<Entity *> entityAdded;
    std::vector<Entity *> entityToDelete;


    int tickCount;

    DataAnalyser dataAnalyser;
    std::chrono::system_clock::time_point tickStart;
    std::chrono::system_clock::time_point tickEnd;


public:
    Farm();
    void InitFromRandom();
    void Tick(bool paused);

    void sortCreatures();


    void populationControl();
    void statistics();
    void aTickHavePassed();


    void generateEntityGrid();


    const std::vector<Life *> &getLifes() const;

    const std::vector<Entity *> &getEntities() const;




    CreatureNursery *getNursery() const;


    std::vector<Life *> getScoreSortedCreatures();

    const DataAnalyser &getDataAnalyser() const;


    const std::vector<std::vector<std::vector<Entity *>>> &getEntityGrid() const;


    Chunk * getChunkAt(int chunkX, int chunkY);

    void generateRandomTerrain(int seed);

    Tile *getTileAt(int tileX, int tileY);


    void handleBigThread();

    std::vector<Life *> fetchLifes();

    void waitForChunkReadyForStatistics();
};


#endif //CREATURES_FARM_H
