//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_GENITALSEVOLUTION_H
#define CREATURES_GENITALSEVOLUTION_H

#include "../Evolution.h"

class GenitalsEvolution: public Evolution {
private:
    float rotation;
public:
    GenitalsEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(GenitalsEvolution * mate);
    std::string getName() override;
};


#endif //CREATURES_GENITALSEVOLUTION_H
