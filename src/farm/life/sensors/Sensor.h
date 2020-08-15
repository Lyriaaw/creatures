//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_SENSOR_H
#define CREATURES_SENSOR_H


#include "../../entities/Entity.h"
#include "../../brain/neurons/InputNeuron.h"
#include "../../Tile.h"

class Sensor {
protected:
    Entity * entity;
    double value;
    InputNeuron * connectedNeuron;
    std::vector<Point> selectedTiles;
public:
    Sensor(Entity * entity);

    double getValue() const;
    void setValue(double value);

    virtual std::string getName() = 0;


    virtual void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) = 0;
    virtual void findSelectedTiles() = 0;
    void clearSelectedTiles();

    const std::vector<Point> &getSelectedTiles() const;

    void passValueToNeuron();

    void setConnectedNeuron(InputNeuron *connectedNeuron);

    void setEntity(Entity *entity);

};


#endif //CREATURES_SENSOR_H
