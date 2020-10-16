//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#ifndef CREATURES_MEMORYMUSCLE_H
#define CREATURES_MEMORYMUSCLE_H


#include "InternalMuscle.h"

class MemoryMuscle: public InternalMuscle {
private:
    double value;
public:
    MemoryMuscle(Entity *entity);

    std::vector<Entity *> executeAction() override;

    std::string getName() override;

    double getValue() const;
};


#endif //CREATURES_MEMORYMUSCLE_H
