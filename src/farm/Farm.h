//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARM_H
#define CREATURES_FARM_H




#include "Entity.h"
#include "Map.h"
#include "Creature.h"
#include "Food.h"
#include "brain/CreatureNursery.h"

class Farm {
private:
    std::vector<BrainConnector *> connectors;
    std::vector<Food *> foods;
    Map map;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;

    CreatureNursery * nursery;

    std::vector<ActionDTO> actions;

    double availableEnergy;

    std::vector<Entity *> toDelete;
    std::vector<Food *> addedEntity;
    std::vector<Creature *> addedCreatures;

    int tickCount;

public:
    Farm();
    void InitFromRandom();
    void InitRandomMap();
    void Tick(bool paused);

    void brainProcessing();
    void brainOutput();
    void moveCreatures();

    void handleActions();
    void executeCreaturesActions();
    void handleMating(BrainConnector * father, int entityId);

    void populationControl();
    void vegetalisation();
    void statistics();
    void aTickHavePassed();

    void removeEnergyFromFarm(double amount);

    void clearDeletedEntities();
    void generateEntityGrid();
    bool isEntityAboutToBeDeleted(int id);

    Creature * getCreatureFromId(int id);
    Entity * getEntityFromId(int id);
    BrainConnector * getConnectorFromId(int id);

    static std::string getHumanReadableEnergy(float givenEnergy) ;


    Map * getMap();


    const std::vector<Food *> &getFoods() const;


    CreatureNursery *getNursery() const;

    const std::vector<BrainConnector *> &getConnectors() const;

    void addConnector(BrainConnector * connector);

    std::vector<Entity *> getAccessibleEntities(Creature * creature);

    const std::vector<Entity *> &getToDelete() const;

    const std::vector<Food *> &getAddedEntity() const;

    void setAddedEntity(const std::vector<Food *> &addedEntity);

    const std::vector<Creature *> &getAddedCreatures() const;

    void setAddedCreatures(const std::vector<Creature *> &addedCreatures);

    void clearAddedCreatures();
    void clearAddedEntities();

    std::vector<BrainConnector *> getScoreSortedCreatures();

};


#endif //CREATURES_FARM_H
