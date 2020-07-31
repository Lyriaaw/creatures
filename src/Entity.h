//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_ENTITY_H
#define CREATURES_ENTITY_H


#include "Point.h"
#include "UiEntity.h"


class Entity {
private:
    Point position;
    float size;
    float color;

    float rotation;
    float speed;

    UiEntity *entityUi;


public:

    Entity(Point point, float size);

    void move();

    float getColor() const;
    void setColor(float color);

    const Point &getPosition() const;
    void setPosition(const Point &position);

    float getSize() const;
    void setSize(float size);

    float getRotation() const;
    void setRotation(float rotation);

    UiEntity getEntityUi() const;
    void setEntityUi(UiEntity entityUi);
};


#endif //CREATURES_ENTITY_H
