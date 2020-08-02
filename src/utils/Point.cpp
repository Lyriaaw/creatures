//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Point.h"
#include "../World.h"


Point::Point(float x, float y) {
    this->x = x;
    this->y = y;
}





float Point::getX() const {
    return x;
}

void Point::setX(float x) {
    Point::x = x;
}

float Point::getY() const {
    return y;
}

void Point::setY(float y) {
    Point::y = y;
}

Point Point::getSimpleCoordinates() {
    return {float(int(this->x / float(CHUNK_SIZE))), float(int(this->y / float(CHUNK_SIZE)))};
}

bool Point::equals(Point point) const {
    return (point.x == this->x && point.y == this->y);
}




