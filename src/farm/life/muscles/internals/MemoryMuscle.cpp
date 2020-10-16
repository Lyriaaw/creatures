//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#include "MemoryMuscle.h"

std::vector<Entity *> MemoryMuscle::executeAction() {
    value = this->getNeurons().at(0)->getValue();

    std::vector<Entity *> empty;
    return empty;
}

MemoryMuscle::MemoryMuscle(Entity *entity) : InternalMuscle(0, entity), value(0) {

}

std::string MemoryMuscle::getName() {
    return "Rotation";
}

double MemoryMuscle::getValue() const {
    return value;
}
