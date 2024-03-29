//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_NEURON_H
#define CREATURES_NEURON_H

#include <iostream>

class Neuron {
protected:
    int generationNumber;

    float value;

    int x;
    int y;

    float hueOutline;
    float brightnessOutline;
    std::string name;

public:
    Neuron();

    virtual float getValue() = 0;

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    float getHueOutline() const;
    void setHueOutline(float hueOutline);

    float getBrightnessOutline() const;
    void setBrightnessOutline(float brightnessOutline);

    const std::string &getName() const;
    void setName(const std::string &name);

    void clear();
    void addValue(float addedValue);

    int getGenerationNumber() const;

    void setGenerationNumber(int generationNumber);

};


#endif //CREATURES_NEURON_H
