//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#ifndef CREATURES_CHUNK_H
#define CREATURES_CHUNK_H


#include <vector>
#include "life/Life.h"
#include "CreatureNursery.h"


class Chunk {
private:
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

public:

    Chunk(Point chunkPosition, CreatureNursery * nursery);

    void generateRandomChunk(int seed, float min, float max);

    Tile *getTileAt(int tileX, int tileY);

    void generateNeighbours();

    void setNeighbour(int it, int jt, Chunk *neighbour);

    void processClimate();

    Tile *getRelativeTile(int tileX, int tileY, bool debug);

    const std::string &getStep() const;

    void waitForNeighbours(std::vector<std::string> requestedSteps);

    const Point &getChunkPosition() const;

    std::vector<Entity *> getRelativeEntities(int tileX, int tileY);
    std::vector<Entity *> getEntitiesAt(int tileX, int tileY);

    const std::vector<ActionDTO> &getActions() const;

    void setActions(const std::vector<ActionDTO> &actions);

    void executeCreaturesActions();

    void handleCaptureHeat(Life *life, ActionDTO action);

    void handleCaptureGround(Life *life, ActionDTO action);

    bool handleDuplication(Life *life);

    bool handleMating(Life *father, int entityId);

    Life *getLifeFromId(int id, bool askNeighbours);

    Entity *getEntityFromId(int id, bool askNeighbours);

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

    void brainProcessing();

    std::vector<Tile *> getAccessibleTiles(std::vector<Point> selectedTiles);

    std::vector<Entity *> getAccessibleEntities(std::vector<Point> selectedTiles);

    void generateEntityGrid();

    void removeDeletedEntities();

    void moveCreatures();

    void clearAddedLifes();

    void clearAddedEntities();

    void clearToDeleteLifes();

    void clearToDeleteEntities();

    void addLife(Life *life);

    void handleEnergyGiveaway();

    void transferLife(Life *life);

    void checkForLifeTransfer(Life *life);

    void processImportedAndExportedLifes();
};


#endif //CREATURES_CHUNK_H
