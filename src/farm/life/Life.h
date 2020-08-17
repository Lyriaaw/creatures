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
#include "EnergyManagement.h"

class Life {
private:
    Entity * entity;

    std::vector<Sensor *> sensors;

    std::vector<InternalMuscle *> internalMuscles;
    std::vector<ExternalMuscle *> externalMuscles;

    Brain * brain;

    EnergyManagement * energyManagement;

    std::string type;

    int lastCaptureGroundage;
public:
    Life(const std::string &type);

    double giveawayEnergy();

    void processSensors(std::vector<Entity *> availableEntities, std::vector<Tile *> availableTiles);
    void processBrain();

    void processSelectedTiles();
    std::vector<Point> getSelectedTiles();

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

    EnergyManagement *getEnergyManagement() const;

    void setEnergyManagement(EnergyManagement *energyManagement);

    void setMass(double newMass);
    double addEnergy(double energy);

    const std::string &getType() const;

};


#endif //CREATURES_LIFE_H
