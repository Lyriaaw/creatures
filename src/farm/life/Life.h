//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_LIFE_H
#define CREATURES_LIFE_H


#include <vector>
#include "sensors/Sensor.h"
#include "muscles/Muscle.h"
#include "muscles/internals/InternalMuscle.h"
#include "muscles/externals/ExternalMuscle.h"
#include "../Tile.h"
#include "../brain/Brain.h"
#include "EnergyCenter.h"
#include "../../mongo/MongoClient.h"
#include <nlohmann/json.hpp>

class Life {
private:
    Entity * entity;

    int farmId;

    std::vector<Entity *> currentAccessibleEntities;
    std::vector<Tile *> currentAccessibleTiles;

    std::vector<Sensor *> sensors;

    std::vector<InternalMuscle *> internalMuscles;
    std::vector<ExternalMuscle *> externalMuscles;

    Brain * brain;

    EnergyCenter * energyCenter;

    bool naturalMating;
    int birthTick;
    std::string mongoId;
    std::vector<int> parentsIds;
    std::vector<int> childrenIds;
public:
    Life(int farmId);

    double giveawayEnergy();

    void saveAccessibleEntities(std::vector<Entity *> availableEntities, std::vector<Tile *> availableTiles);
    void processSensors();
    void processBrain();

    void processSelectedChunks();
    std::vector<Point> getSelectedChunks();

    std::vector<ActionDTO> executeExternalActions();
    std::vector<Entity *> executeInternalActions();




    Entity *getEntity() const;
    Brain *getBrain() const;

    void setBrain(Brain *brain);

    void setEntity(Entity *entity);

    void addSensor(Sensor * sensor);
    void addInternalMuscle(InternalMuscle * muscle);
    void addExternalMuscle(ExternalMuscle * muscle);

    void connectSensorAndMuscles();

    const std::vector<Sensor *> &getSensors() const;

    const std::vector<InternalMuscle *> &getInternalMuscles() const;

    const std::vector<ExternalMuscle *> &getExternalMuscles() const;

    EnergyCenter *getEnergyCenter() const;

    void setEnergyCenter(EnergyCenter *energyCenter);

    double addEnergy(double energyToAdd);

    bool isAlive();

    const std::vector<Entity *> &getCurrentAccessibleEntities() const;

    const std::vector<Tile *> &getCurrentAccessibleTiles() const;

    nlohmann::json initialDataJson();

    nlohmann::json updateDataJson();


    const std::vector<int> &getParentsIds() const;

    const std::vector<int> &getChildrenIds() const;


    void setNaturalMating(bool naturalMating);

    void addChild(int childId);
    void addParent(int parentId);

    nlohmann::json deathDataJSON();

    void saveToMongo(MongoClient * client);

    void recordDeathToMongo(MongoClient *client, int tick);

    int getBirthTick() const;

    void setBirthTick(int birthTick);
};


#endif //CREATURES_LIFE_H
