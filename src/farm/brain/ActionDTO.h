//
// Created by Amalric Lombard de Buffières on 8/4/20.
//

#ifndef CREATURES_ACTIONDTO_H
#define CREATURES_ACTIONDTO_H


#include <string>
#include "../../utils/Point.h"

class ActionDTO {
private:
    int performerId;
    int subjectId;

    std::string type;

    Point tilePosition;
    int tick;

    double pheromoneEmissionColor;

public:
    ActionDTO(int performerId, int subjectId, const std::string &type);

    int getPerformerId() const;

    int getSubjectId() const;

    const std::string &getType() const;

    const Point &getTilePosition() const;

    void setTilePosition(const Point &tilePosition);

    int getTick() const;

    void setTick(int tick);

    double getPheromoneEmissionColor() const;

    void setPheromoneEmissionColor(double pheromoneEmissionColor);

    void setType(const std::string &type);
};


#endif //CREATURES_ACTIONDTO_H
