//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Point.h"


Point::Point(int x, int y) {
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




