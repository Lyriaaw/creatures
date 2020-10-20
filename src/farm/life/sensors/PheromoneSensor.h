//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#ifndef CREATURES_PHEROMONESENSOR_H
#define CREATURES_PHEROMONESENSOR_H


#include "Sensor.h"

class PheromoneSensor: public Sensor {
private:
    float color;
public:
    PheromoneSensor(Entity *entity);

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) override ;

    void findSelectedChunks() override;

    float getColor() const;

    void setColor(float color);

    std::string getName() override;
};


#endif //CREATURES_PHEROMONESENSOR_H
