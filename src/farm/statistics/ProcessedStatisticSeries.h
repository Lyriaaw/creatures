
//
// Created by Amalric Lombard de Buffieres on 09/07/2021.
//

#ifndef CREATURES_PROCESSEDSTATISTICSERIES_H
#define CREATURES_PROCESSEDSTATISTICSERIES_H

#include <thread>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <iostream>
#include <vector>
#include "../../mongo/MongoClient.h"


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

class ProcessedStatisticSeries {
private:
    int farmId;

    int startTick;
    int endTick;

    std::vector<double> data;
    std::vector<double> averagedData;

    std::string dataName;

    MongoClient * mongoClient;

public:
    ProcessedStatisticSeries(int farmId, const std::string &dataName, std::vector<double> data);

    void process();

    void saveToMongo();
};


#endif //CREATURES_PROCESSEDSTATISTICSERIES_H
