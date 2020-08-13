//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#ifndef CREATURES_TILEHEATEVOLUTION_H
#define CREATURES_TILEHEATEVOLUTION_H


#include "../Evolution.h"

class TileHeatEvolution: public Evolution {
public:
    TileHeatEvolution();

    void generateFromRandom(Life *life) override;

    void perform(Life *connector) override;

    std::string describe() override;

    Evolution *generateWithMate(Evolution *mate) override;
};


#endif //CREATURES_TILEHEATEVOLUTION_H
