//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#ifndef CREATURES_CHUNK_H
#define CREATURES_CHUNK_H


#include <vector>
#include "life/Life.h"
#include "CreatureNursery.h"
#include "statistics/DataAnalyser.h"


class Chunk {
private:
    int tick;

    Point chunkPosition;

    std::vector<Life *> lifes;
    std::vector<Entity *> entities;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;

    std::vector<ActionDTO> actions;

    std::vector<std::vector<Tile *>> tiles;
    std::vector<std::vector<Chunk *>> neighbours;

    std::string step;

    CreatureNursery * nursery;

    std::vector<Life *> importedLifes;
    std::vector<Life *> exportedLifes;

    std::vector<Life *> lifesAdded;
    std::vector<Life *> lifesToDelete;

    std::vector<Entity *> entityAdded;
    std::vector<Entity *> entityToDelete;

    DataAnalyser * dataAnalyser;

    std::mutex entity_changes_mutex;

public:

    Chunk(Point chunkPosition, CreatureNursery * nursery, DataAnalyser * dataAnalyser);

    // Main loop
    void generateEntityGrid();
    void handleEnergyGiveaway();
    void processClimate();
    void brainProcessing();
    void executeCreaturesActions();
    void moveCreatures();
    void aTickHavePassed();


    // Find entities and lifes
    Tile *getRelativeTile(int tileX, int tileY, bool debug);
    Tile *getTileAt(int tileX, int tileY);
    std::vector<Entity *> getRelativeEntities(int tileX, int tileY);
    std::vector<Entity *> getEntitiesAt(int tileX, int tileY);
    Life *getLifeFromId(int id, bool askNeighbours);
    Entity *getEntityFromId(int id, bool askNeighbours);
    std::vector<Tile *> getAccessibleTiles(std::vector<Point> selectedTiles);
    std::vector<Entity *> getAccessibleEntities(std::vector<Point> selectedTiles);
    std::vector<Tile *> getAllTiles(std::vector<Point> *visitedPoints);
    std::vector<Life *> getAllLifes(std::vector<Point> *visitedPoints);



    // The neighbourhood
    void generateRandomChunk(int seed, float min, float max);
    void generateNeighbours();
    void setNeighbour(int it, int jt, Chunk *neighbour);
    void waitForNeighbours(std::vector<std::string> requestedSteps);
    void getNeighboursReady(std::vector<Point> *visitedPoints);


    // In and Out
    void removeDeletedEntities();
    void checkForLifeTransfer(Life *life);
    void transferLife(Life *life);
    void addLife(Life *life);
    void processImportedAndExportedLifes();


    // The actions
    void handleCaptureHeat(Life *life, ActionDTO action);
    void handleCaptureGround(Life *life, ActionDTO action);
    bool handleDuplication(Life *life);
    bool handleMating(Life *father, int entityId);


    // For the UI
    void clearAddedLifes();
    void clearAddedEntities();
    void clearToDeleteLifes();
    void clearToDeleteEntities();




    // Java hell
    const std::string &getStep() const;

    const Point &getChunkPosition() const;

    const std::vector<ActionDTO> &getActions() const;

    void setActions(const std::vector<ActionDTO> &actions);

    const std::vector<Life *> &getLifes() const;

    void setLifes(const std::vector<Life *> &lifes);

    const std::vector<Entity *> &getEntities() const;

    void setEntities(const std::vector<Entity *> &entities);

    const std::vector<Life *> &getLifesAdded() const;

    void setLifesAdded(const std::vector<Life *> &lifesAdded);

    const std::vector<Life *> &getLifesToDelete() const;

    void setLifesToDelete(const std::vector<Life *> &lifesToDelete);

    const std::vector<Entity *> &getEntityAdded() const;

    void setEntityAdded(const std::vector<Entity *> &entityAdded);

    const std::vector<Entity *> &getEntityToDelete() const;

    void setEntityToDelete(const std::vector<Entity *> &entityToDelete);

    void setStep(const std::string &step);




    void statistics();
};


#endif //CREATURES_CHUNK_H
