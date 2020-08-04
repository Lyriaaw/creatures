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

    float averageSelectedEntities;

    std::vector<ActionDTO> actions;

public:
    Farm();
    void InitFromRandom();
    void InitRandomMap();
    void Tick(bool paused);
    void generateEntityGrid();
    void executeCreaturesActions();
    void removeEntity(int id);

    Creature * getCreatureFromId(int id);
    Entity * getEntityFromId(int id);


    FarmUI *getUi() const;

    void setUi(FarmUI *ui);

    Map * getMap();


    const std::vector<Food *> &getFoods() const;


    CreatureNursery *getNursery() const;

    const std::vector<BrainConnector *> &getConnectors() const;

    void addConnector(BrainConnector * connector);

    std::vector<Entity *> getAccessibleEntities(Creature * creature);

    float getAverageSelectedEntities() const;

};


#endif //CREATURES_FARM_H
