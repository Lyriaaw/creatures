//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_NEURON_H
#define CREATURES_NEURON_H

#include <iostream>

class Neuron {
protected:
    float value;

    float x;
    float y;

    float hueOutline;
    float brightnessOutline;
    std::string name;

public:
    float getValue() const;

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    float getHueOutline() const;
    void setHueOutline(float hueOutline);

    float getBrightnessOutline() const;
    void setBrightnessOutline(float brightnessOutline);

    const std::string &getName() const;
    void setName(const std::string &name);
};


#endif //CREATURES_NEURON_H
