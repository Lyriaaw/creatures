//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_GENITALSEVOLUTION_H
#define CREATURES_GENITALSEVOLUTION_H

#include "Evolution.h"

class GenitalsEvolution: public Evolution {
private:
    float rotation;
public:
    GenitalsEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(GenitalsEvolution * mate);
};


#endif //CREATURES_GENITALSEVOLUTION_H
