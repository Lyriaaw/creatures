//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_TILE_H
#define CREATURES_TILE_H


#include <mutex>

class Tile {
private:
    float color;
    float height;
    float heat;
    float ground;

    std::mutex heatMutex;
    std::mutex groundMutex;


public:

    Tile(float height, float color, float heat, float ground);

    float getHeight() const;

    void setHeight(float height);

    float getHeat() const;

    void setHeat(float heat);

    float getGround() const;

    void setGround(float ground);

    void addHeight(float value);
    void addHeat(float value);
    void addGround(float value);

    void removeGround(float groundToRemove);

    float getColor() const;

    void setColor(float color);
};


#endif //CREATURES_TILE_H
