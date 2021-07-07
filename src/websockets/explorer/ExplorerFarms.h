
//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#ifndef CREATURES_EXPLORERFARMS_H
#define CREATURES_EXPLORERFARMS_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "../../mongo/MongoClient.h"

using json = nlohmann::json;

class ExplorerFarms {
private:
    json params;
    json body;

    MongoClient * mongoClient;

public:
    ExplorerFarms(MongoClient * mongoClient, json params, json body);

    json handleRequest();

    json handleFarmsRequest();
    json handleGetFarmsRequest();

    json handleFarmRequest();

    json handleNotYetImplemented();

    json handleGetFarmRequest();

    json handleProcessLifeRunnersRequest();

    json handleFetchStatisticsSeries();
};


#endif //CREATURES_EXPLORERFARMS_H
