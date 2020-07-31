//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_ENTITY_H
#define CREATURES_ENTITY_H


#include "../utils/Point.h"
#include "../ui/Camera.h"
#include <random>


class Entity {
public:
    Entity(Point position, float size);



    float getColor() const;

    const Point &getPosition() const;

    float getSize() const;

    float getRotation() const;

    float getSpeed() const;


protected:
    Point position;
    float size;

    float rotation;
    float speed;

    float color;



};


#endif //CREATURES_ENTITY_H
