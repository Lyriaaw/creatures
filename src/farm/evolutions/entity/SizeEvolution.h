//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_SIZEEVOLUTION_H
#define CREATURES_SIZEEVOLUTION_H


#include "../Evolution.h"

class SizeEvolution: public Evolution {
private:
    float size;
public:
    SizeEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(SizeEvolution * mate);

    std::string getName() override;

    void saveInMongo(MongoClient *client, int farmId);

    bsoncxx::builder::stream::document generateMongoVariables() override;
};


#endif //CREATURES_SIZEEVOLUTION_H
