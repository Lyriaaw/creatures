//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_COLOREVOLUTION_H
#define CREATURES_COLOREVOLUTION_H


#include "Evolution.h"

class ColorEvolution: public Evolution {
private:
    float hue;
public:
    ColorEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;

    Evolution * generateFromCastedMate(ColorEvolution * mate);

    void setHue(float hue);


};


#endif //CREATURES_COLOREVOLUTION_H
