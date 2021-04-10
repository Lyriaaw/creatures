//
// Created by Amalric Lombard de Buffières on 10/21/20.
//

#ifndef CREATURES_LIFESRUNNER_H
#define CREATURES_LIFESRUNNER_H


#include <vector>
#include <nlohmann/json.hpp>
#include "life/Life.h"
#include "Map.h"
#include "CreatureNursery.h"
#include "statistics/LifeRunnerDataTracker.h"
#include "../websockets/LifeRunnerWebsocket.h"
using json = nlohmann::json;

class LifesRunner {
private:
    int id;

    std::vector<Life *> lifes;
    std::vector<int> deadLifeIds;

    std::mutex lifes_mutex;

    LifeRunnerDataTracker dataAnalyser;

    LifeRunnerWebsocket *websocket;

    std::function<void(Point, float, float, double, double, double)> generateEntities;
    std::function<std::vector<Entity *>(std::vector<Point> selectedChunks)> getAccessibleEntities;
    std::function<std::vector<Tile *>(Life * life, std::vector<Point> selectedChunks)> getAccessibleTiles;
    std::function<void(ActionDTO)> recordExecutedAction;
    std::function<Life * (int id)> getLifeFromId;
    std::function<void(Life *)> addLifeToFarm;

    CreatureNursery * creatureNursery;

    FarmControl * farmControl;

    std::vector<ActionDTO> actions;

    Map * map;

    int tick;

    std::chrono::system_clock::time_point tickStart;
    std::chrono::system_clock::time_point tickEnd;

    std::function<void(int id)> triggerCreaturesUpdate;

    int medianTick;

public:
    LifesRunner(int id);

    void addLife(Life *life);

    int getLifeCount();

    void brainProcessing(bool paused);
    void triggerUpdate();

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

    std::vector<Life *> getLifes();

    const LifeRunnerDataTracker &getDataAnalyser() const;

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

    nlohmann::basic_json<> asJson();

    void handleThread();

    void setFarmControl(FarmControl *farmControl);

    int getId() const;

    void setId(int id);

    void setTriggerCreaturesUpdate(const std::function<void(int)> &triggerCreaturesUpdate);

    void setMedianTick(int medianTick);

    nlohmann::basic_json<> creaturesAsJson();

    void saveOnMongo();
};


#endif //CREATURES_LIFESRUNNER_H
