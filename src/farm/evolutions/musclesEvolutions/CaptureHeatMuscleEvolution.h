//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#ifndef CREATURES_CAPTUREHEATMUSCLEEVOLUTION_H
#define CREATURES_CAPTUREHEATMUSCLEEVOLUTION_H


#include "../Evolution.h"

class CaptureHeatMuscleEvolution: public Evolution {
private:
    float rotation;
public:
    CaptureHeatMuscleEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_CAPTUREHEATMUSCLEEVOLUTION_H
