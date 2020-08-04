//
// Created by Amalric Lombard de Buffières on 8/4/20.
//

#ifndef CREATURES_ACTIONDTO_H
#define CREATURES_ACTIONDTO_H


#include <string>

class ActionDTO {
private:
    int performerId;
    int subjectId;

    std::string type;

public:
    ActionDTO(int performerId, int subjectId, const std::string &type);

    int getPerformerId() const;

    int getSubjectId() const;

    const std::string &getType() const;
};


#endif //CREATURES_ACTIONDTO_H
