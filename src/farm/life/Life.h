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

class Life {
private:
    Entity * entity;

    std::vector<Sensor *> sensors;

    std::vector<InternalMuscle *> internalMuscles;
    std::vector<ExternalMuscle *> externalMuscles;

    Brain * brain;

    double maxMass;
    double availableEnergy;

public:
    double giveawayEnergy();

    void processSensors(std::vector<Entity *> availableEntities, std::vector<Tile *> availableTiles);
    void processBrain();

    void processSelectedChunks();
    std::vector<Point> getSelectedChunks();

    std::vector<ActionDTO> executeExternalActions(std::vector<Entity *> availableEntities);
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

    double getMaxMass() const;

    void setMaxMass(double maxMass);

    double getAvailableEnergy() const;

    void setAvailableEnergy(double availableEnergy);


    bool isAlive();

    double eat(Entity * eaten);
};


#endif //CREATURES_LIFE_H
