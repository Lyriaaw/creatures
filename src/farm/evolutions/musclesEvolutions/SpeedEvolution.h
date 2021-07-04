//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_SPEEDEVOLUTION_H
#define CREATURES_SPEEDEVOLUTION_H


#include "../Evolution.h"

class SpeedEvolution: public Evolution {
public:
    SpeedEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    std::string getName() override;

    void saveInMongo(MongoClient *client, int farmId);
    bsoncxx::builder::stream::document generateMongoVariables() override;
};


#endif //CREATURES_SPEEDEVOLUTION_H
