//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#include "ExplorerFarms.h"
#include "LifeRunnerDataAnalyser.h"


ExplorerFarms::ExplorerFarms(MongoClient * mongoClient, json params, json body): mongoClient(mongoClient), params(std::move(params)), body(std::move(body)) {

}

json ExplorerFarms::handleNotYetImplemented() {
    return {
            {"error", "not_yet_implemented"},
            {"params", params},
            {"body", body},
    };
}

json ExplorerFarms::handleRequest() {

    std::cout << "Farm request: " << params.dump() << " => " << body.dump() << std::endl;

    auto response = handleNotYetImplemented();

    if (params["endpoint"] == "farms") {
        response = handleFarmsRequest();
    } else if (params["endpoint"] == "farm") {
        response =  handleFarmRequest();
    } else if (params["endpoint"] == "process_life_runners") {
        response =  handleProcessLifeRunnersRequest();
    }



    return {
        {"type", "response"},
        {"request", {
            {"params", params},
        }},
        {"response", response},
    };
}



json ExplorerFarms::handleProcessLifeRunnersRequest() {
    if (params["verb"] != "post") {
        return handleNotYetImplemented();
    }

    std::string farmIdString = body["farm_id"];

    int farmId = std::stoi(farmIdString);

    auto *dataAnalyser = new LifeRunnerDataAnalyser(farmId);
    dataAnalyser->handleTread();

    return {};
}








json ExplorerFarms::handleFarmsRequest() {
    if (params["verb"] == "get") {
        return handleGetFarmsRequest();
    }

    return handleNotYetImplemented();
}

json ExplorerFarms::handleGetFarmsRequest() {
    return mongoClient->fetchFarms();
}





json ExplorerFarms::handleFarmRequest() {
    if (params["verb"] == "get") {
        return handleGetFarmRequest();
    }

    return handleNotYetImplemented();
}

json ExplorerFarms::handleGetFarmRequest() {
    return mongoClient->fetchFarm(body["farm_id"]);
}


