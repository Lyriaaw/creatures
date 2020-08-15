//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_SENSOR_H
#define CREATURES_SENSOR_H


#include "../../entities/Entity.h"
#include "../../brain/neurons/InputNeuron.h"
#include "../../Tile.h"
#include "../Body.h"

class Sensor {
protected:
    Body * entity;
    double value;
    InputNeuron * connectedNeuron;
    std::vector<Point> selectedChunks;
public:
    Sensor(Body * entity);

    double getValue() const;
    void setValue(double value);

    virtual std::string getName() = 0;


    virtual void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) = 0;
    virtual void findSelectedChunks() = 0;
    void clearSelectedChunks();

    const std::vector<Point> &getSelectedChunks() const;

    void setSelectedChunks(const std::vector<Point> &selectedChunks);

    void passValueToNeuron();

    void setConnectedNeuron(InputNeuron *connectedNeuron);

    void setEntity(Body *entity);

};


#endif //CREATURES_SENSOR_H
