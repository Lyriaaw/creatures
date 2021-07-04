//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_LINKEVOLUTION_H
#define CREATURES_LINKEVOLUTION_H


#include "../Evolution.h"
#include <map>

class LinkEvolution: public Evolution {
private:
    int inputGenerationNumber, outputGenerationNumber;
    float weight;
public:
    LinkEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(LinkEvolution * mate);
    std::string getName() override;

    void generateFromNeurons(Life * life, Neuron * input, Neuron * output);

    int getInputGenerationNumber() const;

    int getOutputGenerationNumber() const;

    void setWeight(float weight);

    float getWeight() const;

    void changeEvolutionNumberIfAlreadyExists(std::map<int, Evolution *> currentWholeGenome);

    void saveInMongo(MongoClient *client, int farmId);
    bsoncxx::builder::stream::document generateMongoVariables() override;

};


#endif //CREATURES_LINKEVOLUTION_H
