//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARM_H
#define CREATURES_FARM_H




#include "Entity.h"
#include "../ui/FarmUI.h"
#include "../World.h"
#include "Map.h"
#include "Creature.h"

class Farm {
private:
    std::vector<Creature *> creatures;
    FarmUI ui;
    Map map;

public:
    Farm();
    void InitFromRandom();
    void InitRandomMap();
    void Tick();


    const FarmUI &getUi() const;

    void setUi(FarmUI ui);

    Map getMap();

    const std::vector<Creature *> &getCreatures() const;

    void setCreatures(const std::vector<Creature *> &creatures);
};


#endif //CREATURES_FARM_H
