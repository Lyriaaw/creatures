//
// Created by Amalric Lombard de Buffières on 8/4/20.
//

#include "ActionDTO.h"

ActionDTO::ActionDTO(int performerId, int subjectId, const std::string &type) : performerId(performerId),
                                                                                subjectId(subjectId), type(type) {}

int ActionDTO::getPerformerId() const {
    return performerId;
}

int ActionDTO::getSubjectId() const {
    return subjectId;
}

const std::string &ActionDTO::getType() const {
    return type;
}
