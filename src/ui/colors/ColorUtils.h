//
// Created by Amalric Lombard de Buffières on 7/31/20.
//




#include "RGBColor.h"

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

RGBColor hslToRgb(float hue, float saturation, float light) {
    if (saturation == 0) {
        return RGBColor(light * 255, light * 255, light * 255);
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

    float red = getDefinitiveValue(tmpR, temp1, temp2);
    float green = getDefinitiveValue(tmpG, temp1, temp2);
    float blue = getDefinitiveValue(tmpB, temp1, temp2);


    return RGBColor(red * 255, green  * 255, blue  * 255);
}

