//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Point.h"


Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}





int Point::getX() const {
    return x;
}

void Point::setX(int x) {
    Point::x = x;
}

int Point::getY() const {
    return y;
}

void Point::setY(int y) {
    Point::y = y;
}




