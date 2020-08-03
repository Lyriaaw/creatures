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
#include "brain/CreatureNursery.h"

class Farm {
private:
    std::vector<BrainConnector *> connectors;
    std::vector<Food *> foods;
    FarmUI * ui;
    Map map;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;

    CreatureNursery * nursery;

public:
    Farm();
    void InitFromRandom();
    void InitRandomMap();
    void Tick(bool paused);
    void generateEntityGrid();


    FarmUI *getUi() const;

    void setUi(FarmUI *ui);

    Map * getMap();


    const std::vector<Food *> &getFoods() const;


    CreatureNursery *getNursery() const;

    const std::vector<BrainConnector *> &getConnectors() const;

    void addConnector(BrainConnector * connector);
};


#endif //CREATURES_FARM_H
