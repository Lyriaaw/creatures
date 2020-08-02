//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#ifndef CREATURES_CREATURE_H
#define CREATURES_CREATURE_H


#include "Entity.h"

class Creature: public Entity {
public:
    Creature(Point position, float size);
    void move();
    int getSensorCount();

    float getSensorRotation(int index);
    float getSensorLength(int index);
    float getSensorColor(int index);

    float getSensorDistance(int index);
    float getSensorBrightness(int index);

    void findSelectedChunks();
    void getSensorCoordinates(std::vector<Entity *> entites);



    int addVisionSensorSensor(float length, float rotation, float color);
    const std::vector<Point> &getSelectedChunks() const;


//    std::vector<Evolution *> evolutions;


private:

    std::vector<float> sensorRotations;
    std::vector<float> sensorLengths;
    std::vector<float> sensorColors;

    std::vector<float> sensorDistances;
    std::vector<float> sensorBrightness;

    std::vector<Point> selectedChunks;

    void getSensorValueFromSensorEquation(int sensorIndex, float sensorX, float sensorY, float m, float p, std::vector<Entity *> accessibleEntities);
};


#endif //CREATURES_CREATURE_H
