//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_ENTITY_H
#define CREATURES_ENTITY_H


#include "../../ui/components/Camera.h"
#include "../../World.h"
#include <random>


class Entity {
public:
    Entity(Point position);




    float getColor() const;

    void setColor(float color);

    const Point &getPosition() const;

    double getSize() const;

    float getRotation() const;

    float getSpeed() const;
    float getBrightness() const;

    void setRotation(float rotation);

    void setSpeed(float speed);

    void setPosition(const Point &position);

    Point getSimpleCoordinates();

    int getId() const;

    bool isExists() const;

    void setExists(bool exists);

    float getMass() const;
    double setMass(double energy);
    double removeMass(double removedMass);

    int getAge() const;
    void setAge(int age);
    void aTickHavePassed();

    void setBrightness(float brightness);


protected:
    static int GLOBAL_INDEX;

    int id;

    Point position;

    float rotation;
    float speed;

    float color;
    float brightness;

    double mass;

    bool exists;

    int age;

};


#endif //CREATURES_ENTITY_H
