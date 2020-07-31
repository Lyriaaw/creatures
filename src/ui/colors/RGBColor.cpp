//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#include "RGBColor.h"

int RGBColor::getRed() const {
    return red;
}

void RGBColor::setRed(int red) {
    RGBColor::red = red;
}

int RGBColor::getGreen() const {
    return green;
}

void RGBColor::setGreen(int green) {
    RGBColor::green = green;
}

int RGBColor::getBlue() const {
    return blue;
}

void RGBColor::setBlue(int blue) {
    RGBColor::blue = blue;
}

RGBColor::RGBColor(int red, int green, int blue) : red(red), green(green), blue(blue) {}
