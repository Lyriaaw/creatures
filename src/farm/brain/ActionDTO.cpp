//
// Created by Amalric Lombard de Buffières on 8/4/20.
//

#include "ActionDTO.h"

ActionDTO::ActionDTO(int performerId, int subjectId, const std::string &type) : performerId(performerId),
                                                                                subjectId(subjectId), type(type), tilePosition(Point(0, 0)) {}

int ActionDTO::getPerformerId() const {
    return performerId;
}

int ActionDTO::getSubjectId() const {
    return subjectId;
}

const std::string &ActionDTO::getType() const {
    return type;
}

const Point &ActionDTO::getTilePosition() const {
    return tilePosition;
}

void ActionDTO::setTilePosition(const Point &tilePosition) {
    ActionDTO::tilePosition = tilePosition;
}

int ActionDTO::getTick() const {
    return tick;
}

void ActionDTO::setTick(int tick) {
    ActionDTO::tick = tick;
}

double ActionDTO::getPheromoneEmissionColor() const {
    return pheromoneEmissionColor;
}

void ActionDTO::setPheromoneEmissionColor(double pheromoneEmissionColor) {
    ActionDTO::pheromoneEmissionColor = pheromoneEmissionColor;
}

void ActionDTO::setType(const std::string &type) {
    ActionDTO::type = type;
}
