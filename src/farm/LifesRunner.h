//
// Created by Amalric Lombard de Buffières on 10/21/20.
//

#ifndef CREATURES_LIFESRUNNER_H
#define CREATURES_LIFESRUNNER_H


#include <vector>
#include "life/Life.h"
#include "statistics/DataAnalyser.h"
#include "Map.h"
#include "CreatureNursery.h"

class LifesRunner {
private:
    std::vector<Life *> lifes;

    std::mutex lifes_mutex;

    DataAnalyser dataAnalyser;

    std::function<void(Point, float, float, double, double, double)> generateEntities;
    std::function<std::vector<Entity *>(std::vector<Point> selectedChunks)> getAccessibleEntities;
    std::function<std::vector<Tile *>(Life * life, std::vector<Point> selectedChunks)> getAccessibleTiles;
    std::function<void(ActionDTO)> recordExecutedAction;
    std::function<Life * (int id)> getLifeFromId;
    std::function<void(Life *)> addLifeToFarm;

    CreatureNursery * creatureNursery;


    std::vector<ActionDTO> actions;

    Map * map;

    int tick;

public:
    LifesRunner();

    void addLife(Life *life);

    int getLifeCount();

    void brainProcessing(bool paused);

    void moveCreatures();

    std::vector<Life *> removeDeadLifes();

    void checkAndHandleLifeDying(Life *life);

    void setGenerateEntities(const std::function<void(Point, float, float, double, double, double)> &generateEntities);

    void
    setGetAccessibleEntities(const std::function<std::vector<Entity *>(std::vector<Point>)> &getAccessibleEntities);

    void
    setGetAccessibleTiles(const std::function<std::vector<Tile *>(Life *, std::vector<Point>)> &getAccessibleTiles);

    void setRecordExecutedAction(const std::function<void(ActionDTO)> &recordExecutedAction);

    void setGetLifeFromId(const std::function<Life *(int)> &getLifeFromId);

    const std::vector<ActionDTO> &getActions() const;

    void clearActions();

    void setMap(Map *map);

    const std::vector<Life *> &getLifes() const;

    const DataAnalyser &getDataAnalyser() const;

    void handlePoop(Life *subject);

    int getTick() const;

    void setTick(int tick);

    void handleEating(Life *performer, Entity *subject);

    void handleBiting(Life *performer, Entity *subject);

    void handlePheromoneEmission(Life *performer, ActionDTO action);

    void handleBitingLife(Life *performer, ActionDTO action);

    void handleEatLife(Life *performer, ActionDTO action);

    bool handleMating(Life *father, ActionDTO action);

    void setAddLifeToFarm(const std::function<void(Life *)> &addLifeToFarm);

    void setCreatureNursery(CreatureNursery *creatureNursery);

    void executeCreatureActions();

    void recordMatingFailure(Life *performer);
};


#endif //CREATURES_LIFESRUNNER_H
