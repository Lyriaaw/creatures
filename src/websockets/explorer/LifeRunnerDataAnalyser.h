
//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#ifndef CREATURES_LIFERUNNERDATAANALYSER_H
#define CREATURES_LIFERUNNERDATAANALYSER_H


#include "../../mongo/MongoClient.h"
#include "../../farm/statistics/ProcessedStatisticSeries.h"
#include <thread>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

class LifeRunnerDataAnalyser {
private:
    MongoClient * mongoClient;
    int farmId;
    int tickCount;

public:
    LifeRunnerDataAnalyser(int farmId);
    void handleTread();

    void handleProcess();

    void saveFarmDataProcessProgress(std::string step, double progressionPercentage, int farmId);

    std::vector<ProcessedStatisticSeries *>  processCreatures();
};


#endif //CREATURES_LIFERUNNERDATAANALYSER_H
