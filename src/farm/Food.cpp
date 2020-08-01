//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include "Food.h"

Food::Food(Point position, float size): Entity(position, size) {
    this->color = 0.28;
    this->brightness = 0.2f;
}