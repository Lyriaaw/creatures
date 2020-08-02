//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#ifndef CREATURES_CREATURE_H
#define CREATURES_CREATURE_H


#include "Entity.h"
#include "brain/Brain.h"

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
    void getSensorCoordinates(std::vector<Entity *> entites, Creature * selectedCreature);

    int addVisionSensorSensor(float length, float rotation, float color);
    const std::vector<Point> &getSelectedChunks() const;

    Brain *getBrain() const;
    void setBrain(Brain *brain);

//    std::vector<Evolution *> evolutions;


private:
    Brain * brain;


    std::vector<float> sensorRotations;
    std::vector<float> sensorLengths;
    std::vector<float> sensorColors;

    std::vector<float> sensorDistances;
    std::vector<float> sensorBrightness;

    std::vector<Point> selectedChunks;

    void getSensorValueFromSensorEquation(int sensorIndex, float sensorX, float sensorY, float m, float p, std::vector<Entity *> accessibleEntities);
};


#endif //CREATURES_CREATURE_H
