//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_SIZEEVOLUTION_H
#define CREATURES_SIZEEVOLUTION_H


#include "../Evolution.h"

class SizeEvolution: public Evolution {
private:
    float size;
public:
    SizeEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(SizeEvolution * mate);
};


#endif //CREATURES_SIZEEVOLUTION_H
