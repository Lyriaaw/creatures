//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_ENTITY_H
#define CREATURES_ENTITY_H


#include "../ui/Camera.h"
#include "../World.h"
#include <random>


class Entity {
public:
    Entity(Point position, float size);



    float getColor() const;

    const Point &getPosition() const;

    float getSize() const;

    float getRotation() const;

    float getSpeed() const;
    float getBrightness() const;

    void setRotation(float rotation);

    void setSpeed(float speed);

    void setPosition(const Point &position);

    Point getSimpleCoordinates();

    int getId() const;


protected:
    static int GLOBAL_INDEX;

    int id;

    Point position;
    float size;

    float rotation;
    float speed;

    float color;
    float brightness;


};


#endif //CREATURES_ENTITY_H
