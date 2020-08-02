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

float checkIfBetween0And1(float value) {
    if (value > 1)
        value -= 1;

    if (value < 0)
        value += 1;

    return value;
}

float getDefinitiveValue(float value, float tmp1, float tmp2) {
    if (6 * value < 1) {
        return tmp2 + ((tmp1 - tmp2) * 6 * value);
    }

    if (2 * value < 1) {
        return tmp1;
    }

    if (3 * value < 2) {
        return tmp2 + ((tmp1 - tmp2) * 6 * (0.6666 - value));
    }

    return tmp2;
}

RGBColor::RGBColor(float hue, float saturation, float light) {
    if (saturation == 0) {
        red = 255;
        green = 255;
        blue = 255;
    }

    float temp1;
    if (light < 0.5) {
        temp1 = light * (1 + saturation);
    } else {
        temp1 = light + saturation - (light * saturation);
    }

    float temp2 = 2 * light - temp1;

    float tmpR = hue + 0.333f;
    float tmpG = hue;
    float tmpB = hue - 0.333f;

    tmpR = checkIfBetween0And1(tmpR);
    tmpG = checkIfBetween0And1(tmpG);
    tmpB = checkIfBetween0And1(tmpB);

    red = getDefinitiveValue(tmpR, temp1, temp2) * 255;
    green = getDefinitiveValue(tmpG, temp1, temp2) * 255;
    blue = getDefinitiveValue(tmpB, temp1, temp2) * 255;
}

