//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#ifndef CREATURES_CREATURE_H
#define CREATURES_CREATURE_H


#include "Entity.h"

class Creature: public Entity {
public:
    Creature(Point position);
    void move();
    int getSensorCount();

    float getSensorRotation(int index);
    float getSensorLength(int index);
    float getSensorColor(int index);

    float getSensorDistance(int index);
    float getSensorBrightness(int index);
    float getSensorSizes(int index);


    void findSelectedChunks();
    void processSensorsValues(std::vector<Entity *> entites);
    void executeAction(std::vector<Entity *> entites);

    void addSpeed(float speedToAdd);
    void addRotation(float rotationToAdd);

    float getMouthRotation() const;
    void setMouthRotation(float mouthRotation);

    float getGenitalsRotation() const;
    void setGenitalsRotation(float genitalsRotation);

    float getMouthValue() const;

    void setMouthValue(float mouthValue);

    float getGenitalsValue() const;

    void setGenitalsValue(float genitalsValue);


    int addVisionSensorSensor(float length, float rotation, float color);
    const std::vector<Point> &getSelectedChunks() const;


//    std::vector<Evolution *> evolutions;


private:

    std::vector<float> sensorRotations;
    std::vector<float> sensorLengths;
    std::vector<float> sensorColors;

    std::vector<float> sensorDistances;
    std::vector<float> sensorBrightness;
    std::vector<float> sensorSizes;

    std::vector<Point> selectedChunks;

    float mouthRotation;
    float genitalsRotation;

    float mouthValue;
    float genitalsValue;

    void getSensorValueFromSensorEquation(int sensorIndex, float sensorX, float sensorY, float m, float p, std::vector<Entity *> accessibleEntities);
};


#endif //CREATURES_CREATURE_H
