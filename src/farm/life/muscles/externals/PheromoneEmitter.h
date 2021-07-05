//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#ifndef CREATURES_PHEROMONEEMITTER_H
#define CREATURES_PHEROMONEEMITTER_H


#include "ExternalMuscle.h"

class PheromoneEmitter: public ExternalMuscle {
private:
    double color;
public:
    PheromoneEmitter(float rotation, Entity *entity);

    std::vector<ActionDTO> prepareActionDTO(std::vector<Entity *> accessibleEntities) override;

    std::string getName() override;

    void setColor(double color);

    nlohmann::json toJson();
};


#endif //CREATURES_PHEROMONEEMITTER_H
