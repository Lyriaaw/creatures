//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARM_H
#define CREATURES_FARM_H

#define CHUNK_SIZE 60

#define CHUNK_COUNT_WIDTH 16 * 10
#define CHUNK_COUNT_HEIGHT 16 * 10

#define FARM_WIDTH CHUNK_COUNT_WIDTH * CHUNK_SIZE
#define FARM_HEIGHT CHUNK_COUNT_HEIGHT * CHUNK_SIZE


#include "../Entity.h"
#include "../ui/FarmUI.h"

class Farm {
private:
    float map[CHUNK_COUNT_WIDTH][CHUNK_COUNT_HEIGHT];
    std::vector<Entity> entities;
    FarmUI *ui;


public:
    Farm();
    void InitFromRandom();
    void InitRandomMap();
    void Tick();


    float getMapAt(int chunkX, int chunkY);

    FarmUI *getUi() const;

    void setUi(FarmUI *ui);

    const std::vector<Entity> &getEntities() const;
    void setEntities(const std::vector<Entity> &entities);

};


#endif //CREATURES_FARM_H
