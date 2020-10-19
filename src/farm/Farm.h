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
#include <thread>

class Farm {
private:
    std::vector<Life *> sorted;

    std::vector<Life *> lifes;
    std::vector<Entity *> entities;

    Map * map;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;

    CreatureNursery * nursery;

    std::vector<ActionDTO> actions;
    std::vector<ActionDTO> executedActions;

    double availableEnergy;


    std::vector<Life *> lifesAdded;
    std::vector<Life *> lifesToDelete;

    std::vector<Entity *> entityAdded;
    std::vector<Entity *> entityToDelete;


    int tickCount;

    DataAnalyser dataAnalyser;
    std::chrono::system_clock::time_point tickStart;
    std::chrono::system_clock::time_point tickEnd;

//    std::vector<std::thread> lifeThreads;
    std::mutex actions_mutex;
    std::mutex executed_actions_mutex;

    std::mutex add_mutex;
    std::mutex delete_mutex;


public:
    Farm();
    void InitFromRandom();
    void Tick(bool paused);

    void sortCreatures();

    void brainProcessing();
    void moveCreatures();

    void executeCreaturesActions();
    bool handleMating(Life * father, int entityId);

    void populationControl();
    void vegetalisation();
    void statistics();
    void aTickHavePassed();


    void removeDeletedEntities();
    void clearToDelete();
    void generateEntityGrid();

    Entity * getEntityFromId(int id);
    Life * getLifeFromId(int id);

    const std::vector<Life *> &getLifes() const;

    const std::vector<Entity *> &getEntities() const;

    static std::string getHumanReadableEnergy(float givenEnergy) ;


    Map *getMap() const;



    CreatureNursery *getNursery() const;


    void addLife(Life * life);

    std::vector<Entity *> getAccessibleEntities(std::vector<Point> selectedChunks);
    std::vector<Tile *> getAccessibleTiles(std::vector<Point> selectedChunks);

    const std::vector<Life *> &getLifesAdded() const;
    const std::vector<Entity *> &getEntityAdded() const;
    const std::vector<Entity *> &getEntityToDelete() const;
    const std::vector<Life *> &getLifesToDelete() const;


    void clearAddedLifes();
    void clearAddedEntities();
    void clearToDeleteLifes();
    void clearToDeleteEntities();

    std::vector<Life *> getScoreSortedCreatures();

    const DataAnalyser &getDataAnalyser() const;

    void setDataAnalyser(const DataAnalyser &dataAnalyser);

    const std::vector<std::vector<std::vector<Entity *>>> &getEntityGrid() const;


    void multithreadBrainProcessing(bool paused);

    void addActions(std::vector<ActionDTO> actions);

    void handleDecay();

    void handlePoop(Life *subject);

    void handleBiting(Life *performer, Entity *subject);
    void handleEating(Life *performer, Entity *subject);

    void checkAndHandleLifeDying(Life *life);

    int getTickCount() const;

    void generateEntities(Point position, float color, float brightness, double maxSize, double totalEnergy, double spreadingRatio);

    void analyseColors();

    std::vector<Entity *> getAndClearEntitiesToDelete();
    std::vector<Life *> getAndClearLifesToDelete();

    std::vector<Entity *> getAndClearEntitiesToAdd();

    std::vector<Life *> getAndClearLifesToAdd();

    std::vector<ActionDTO> getAndClearExecutedActions();

    void selectedCreatureChange(std::string type);

    void addEntitiesToFarm(std::vector<Entity *> addedEntities);

    void addLifesToFarm(Life * newLife);
};


#endif //CREATURES_FARM_H
