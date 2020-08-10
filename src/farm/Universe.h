//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_UNIVERSE_H
#define CREATURES_UNIVERSE_H


#include "life/Life.h"

class Universe {
private:
    std::vector<Life *> lifes;
    std::vector<Entity *> entities;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;



public:
    void bigBang();
    void tick();

};


#endif //CREATURES_UNIVERSE_H
