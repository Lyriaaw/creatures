//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#ifndef CREATURES_DISTANCEBARSENSOREVOLUTION_H
#define CREATURES_DISTANCEBARSENSOREVOLUTION_H


#include "../Evolution.h"

class DistanceBarSensorEvolution: public Evolution {
public:
    DistanceBarSensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(DistanceBarSensorEvolution * mate);
    std::string getName() override;

    void setSensorLength(float sensorLength);

    void setSensorRotation(float sensorRotation);
    void saveInMongo(MongoClient *client, int farmId);
    bsoncxx::builder::stream::document generateMongoVariables() override;


private:
    float sensorLength;
    float sensorRotation;

};


#endif //CREATURES_DISTANCEBARSENSOREVOLUTION_H
