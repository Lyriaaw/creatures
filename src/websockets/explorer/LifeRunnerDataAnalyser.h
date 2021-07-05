
//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#ifndef CREATURES_LIFERUNNERDATAANALYSER_H
#define CREATURES_LIFERUNNERDATAANALYSER_H


#include "../../mongo/MongoClient.h"
#include <thread>

class LifeRunnerDataAnalyser {
private:
    MongoClient * mongoClient;
    int farmId;

public:
    LifeRunnerDataAnalyser(int farmId);
    void handleTread();

    void handleProcess();

    void saveFarmDataProcessProgress(std::string step, double progressionPercentage, int farmId);
};


#endif //CREATURES_LIFERUNNERDATAANALYSER_H
