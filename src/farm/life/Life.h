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
#include "Body.h"

class Life {
private:
    Body * body;

    std::vector<Sensor *> sensors;

    std::vector<InternalMuscle *> internalMuscles;
    std::vector<ExternalMuscle *> externalMuscles;

    Brain * brain;

public:
    double giveawayEnergy();
    double addEnergy(double addedEnergy);

    void processSensors(std::vector<Entity *> availableEntities, std::vector<Tile *> availableTiles);
    void processBrain();

    void processSelectedChunks();
    std::vector<Point> getSelectedChunks();

    std::vector<ActionDTO> executeExternalActions(std::vector<Entity *> availableEntities);
    std::vector<Entity *> executeInternalActions();


    Body *getBody() const;
    Body *getEntity() const;

    Brain *getBrain() const;

    void setBrain(Brain *brain);

    void setEntity(Body *entity);

    void addSensor(Sensor * sensor);
    void addInternalMuscle(InternalMuscle * muscle);
    void addExternalMuscle(ExternalMuscle * muscle);

    void connectSensorAndMuscles();

    const std::vector<Sensor *> &getSensors() const;

    const std::vector<InternalMuscle *> &getInternalMuscles() const;

    const std::vector<ExternalMuscle *> &getExternalMuscles() const;

};


#endif //CREATURES_LIFE_H
