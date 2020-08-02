//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#ifndef CREATURES_RGBCOLOR_H
#define CREATURES_RGBCOLOR_H


class RGBColor {
private:
    int red;
    int green;
    int blue;

public:
    RGBColor (float hue, float saturation, float light);
    RGBColor(int red, int green, int blue);

    int getRed() const;

    void setRed(int red);

    int getGreen() const;

    void setGreen(int green);

    int getBlue() const;

    void setBlue(int blue);
};


#endif //CREATURES_RGBCOLOR_H
