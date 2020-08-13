//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#ifndef CREATURES_CAPTUREGROUNDMUSCLEEVOLUTION_H
#define CREATURES_CAPTUREGROUNDMUSCLEEVOLUTION_H


#include "../Evolution.h"

class CaptureGroundMuscleEvolution: public Evolution {
private:
    float rotation;
public:
    CaptureGroundMuscleEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_CAPTUREGROUNDMUSCLEEVOLUTION_H
