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
    float getSensorDistance(int index);

    void findSelectedChunks();
    void getSensorCoordinates(std::vector<Entity *> entites, Creature * selectedCreature);

    const std::vector<Point> &getSelectedChunks() const;
    const std::vector<Entity *> &getAccessibleEntities() const;


private:
    std::vector<float> sensorRotations;
    std::vector<float> sensorLengths;

    std::vector<float> sensorDistances;
    std::vector<float> sensorColors;
    std::vector<float> sensorHue;

    std::vector<Point> selectedChunks;

    std::vector<Entity *> accessibleEntities;

};


#endif //CREATURES_CREATURE_H
