//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_BARSENSOR_H
#define CREATURES_BARSENSOR_H


#include "../Sensor.h"
#include "../../../Tile.h"

class BarSensor: public Sensor {
protected:
    double rotation;
    double length;

    double lengthRatio;

public:
    BarSensor(Entity *entity, double rotation, double length);

    void findSelectedChunks() override;
    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) override;
    void getSensorValueFromSensorEquation(float sensorX, float sensorY, double m, double p, std::vector<Entity *> accessibleEntities);

    virtual void processSensorValue(double distance, Entity * closestEntity) = 0;

    double getRotation() const;

    double getLength() const;

    double getLengthRatio() const;

    void setLength(double length);

    void setLengthRatio(double lengthRatio);

};


#endif //CREATURES_BARSENSOR_H
