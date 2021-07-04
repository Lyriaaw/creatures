//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_COLOREVOLUTION_H
#define CREATURES_COLOREVOLUTION_H


#include "../Evolution.h"

class ColorEvolution: public Evolution {
private:
    float hue;
public:
    ColorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;

    Evolution * generateFromCastedMate(ColorEvolution * mate);

    void setHue(float hue);

    std::string getName() override;


    void saveInMongo(MongoClient *client, int farmId);

    bsoncxx::builder::stream::document generateMongoVariables() override;
};


#endif //CREATURES_COLOREVOLUTION_H
