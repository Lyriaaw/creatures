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

    float getBrightness() const;

    void setRotation(float rotation);

    void setPosition(const Point &position);

    Point getSimpleCoordinates();

    int getId() const;


    bool isExists() const;

    void setExists(bool exists);

    double getMass() const;
    double setMass(double energy);
    bool haveMass();
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

    float color;
    float brightness;

    double mass;

    bool exists;

    int age;

};


#endif //CREATURES_ENTITY_H
