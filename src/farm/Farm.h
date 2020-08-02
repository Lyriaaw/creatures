//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARM_H
#define CREATURES_FARM_H




#include "Entity.h"
#include "../ui/FarmUI.h"
#include "Map.h"
#include "Creature.h"
#include "Food.h"

class Farm {
private:
    std::vector<Creature *> creatures;
    std::vector<Food *> foods;
    FarmUI ui;
    Map map;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;

public:
    Farm();
    void InitFromRandom();
    void InitRandomMap();
    void Tick(bool paused, Creature * selectedCreature);
    void generateEntityGrid();



    const FarmUI &getUi() const;

    void setUi(FarmUI ui);

    Map * getMap();

    const std::vector<Creature *> &getCreatures() const;

    const std::vector<Food *> &getFoods() const;

    void setCreatures(const std::vector<Creature *> &creatures);
};


#endif //CREATURES_FARM_H
