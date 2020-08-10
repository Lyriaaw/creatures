//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_SENSOR_H
#define CREATURES_SENSOR_H


#include "../../entities/Entity.h"
#include "../../brain/neurons/InputNeuron.h"

class Sensor {
protected:
    Entity * entity;
    double value;
    InputNeuron * connectedNeuron;
    std::vector<Point> selectedChunks;
public:
    Sensor(Entity * entity);

    double getValue() const;
    void setValue(double value);

    virtual std::string getName() = 0;


    virtual void fetchSensorValue(std::vector<Entity *> accessibleEntities) = 0;
    virtual void findSelectedChunks() = 0;
    void clearSelectedChunks();

    const std::vector<Point> &getSelectedChunks() const;

    void setSelectedChunks(const std::vector<Point> &selectedChunks);

    void passValueToNeuron();

    void setConnectedNeuron(InputNeuron *connectedNeuron);

};


#endif //CREATURES_SENSOR_H
