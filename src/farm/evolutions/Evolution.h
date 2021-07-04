//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_EVOLUTION_H
#define CREATURES_EVOLUTION_H


#include "../entities/Creature.h"
#include "../brain/BrainConnector.h"
#include "../life/Life.h"
#include "../../mongo/MongoClient.h"
#include <thread>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

class Evolution {
protected:
    int generationNumber;

    InputNeuron * inputNeuron;
    std::vector<OutputNeuron *> outputNeurons;

    bool enabled;

public:
    static int nextEvolutionNumber;

    Evolution();

    virtual void generateFromRandom(Life * life) = 0;

    virtual void perform(Life * connector) = 0;

    virtual std::string describe() = 0;

    virtual std::string getName() = 0;

    virtual Evolution * generateWithMate(Evolution * mate) = 0;

    virtual void saveInMongo(MongoClient * client, int farmId) = 0;
    virtual bsoncxx::builder::stream::document generateMongoVariables() = 0;


    int getGenerationNumber();

    void setGenerationNumber(int generationNumber);
    void generateGenerationNumber();

    InputNeuron *getInputNeuron() const;

    const std::vector<OutputNeuron *> &getOutputNeurons() const;

    void disable();

    bool isEnabled() const;


};




#endif //CREATURES_EVOLUTION_H
