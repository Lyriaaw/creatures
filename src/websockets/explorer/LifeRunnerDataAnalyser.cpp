//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#include "LifeRunnerDataAnalyser.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <iostream>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

LifeRunnerDataAnalyser::LifeRunnerDataAnalyser(int farmId): farmId(farmId), mongoClient(new MongoClient()) {

}

void LifeRunnerDataAnalyser::handleTread() {
    auto f = [&]() {
        this->handleProcess();
    };

    std::thread lifeRunnerAnalyserThread(f);
    lifeRunnerAnalyserThread.detach();
}

void LifeRunnerDataAnalyser::saveFarmDataProcessProgress(std::string step, double progressionPercentage, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};
    builder << "$set" << bsoncxx::builder::stream::open_document
            << "data_processing" << open_document
            << "step" << step
            << "percentage" << progressionPercentage
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc_value = builder  << bsoncxx::builder::stream::finalize;


    mongoClient->updateFarm(doc_value.view(), farmId);
}

void LifeRunnerDataAnalyser::handleProcess() {
    saveFarmDataProcessProgress("Processing", 0, farmId);

    auto order = document{} << "$natural" << -1 << finalize;
    auto opts = mongocxx::options::find{};
    opts.sort(order.view());

    saveFarmDataProcessProgress("Fetching Mongo Documents", 0, farmId);

    auto lifeRunnersCollection = mongoClient->getLifeRunners();

    auto lifeRunnersDocumentsCount = lifeRunnersCollection.count_documents(document{} << "farmId" << farmId << finalize);

    mongocxx::cursor cursor = lifeRunnersCollection.find(document{} << "farmId" << farmId << finalize, opts);

    std::vector<double> tps;
    std::vector<int> runnersCount;
    std::vector<int> creaturesCount;

    std::vector<int> poopCount;
    std::vector<int> pheromoneCount;
    std::vector<int> eatCount;
    std::vector<int> eatLifeCount;
    std::vector<int> mateFailureCount;
    std::vector<int> mateSuccessCount;
    std::vector<int> biteCount;
    std::vector<int> biteLifeCount;

    std::vector<double> chunkSelection;
    std::vector<double> accessibleEntities;
    std::vector<double> sensorProcessing;
    std::vector<double> brainProcessing;
    std::vector<double> externalActions;
    std::vector<double> moveCreatures;
    std::vector<double> tickTime;


    int tickCount = 0;
    int index = 0;

    for(auto doc : cursor) {
        nlohmann::json lifeRunnerDataJSON = nlohmann::json::parse(bsoncxx::to_json(doc));

        auto tick = lifeRunnerDataJSON["tick"].dump();
        int tickInt = std::stoi(tick);

        if (index == 0) {
            tickCount = tickInt;
            tps.resize(tickInt, 0);
            runnersCount.resize(tickInt, 0);
            creaturesCount.resize(tickInt, 0);

            poopCount.resize(tickInt, 0);
            pheromoneCount.resize(tickInt, 0);
            eatCount.resize(tickInt, 0);
            eatLifeCount.resize(tickInt, 0);
            mateFailureCount.resize(tickInt, 0);
            mateSuccessCount.resize(tickInt, 0);
            biteCount.resize(tickInt, 0);
            biteLifeCount.resize(tickInt, 0);

            chunkSelection.resize(tickInt, 0);
            accessibleEntities.resize(tickInt, 0);
            sensorProcessing.resize(tickInt, 0);
            brainProcessing.resize(tickInt, 0);
            externalActions.resize(tickInt, 0);
            moveCreatures.resize(tickInt, 0);
            tickTime.resize(tickInt, 0);

        }

        if (index % 100 == 0) {
            saveFarmDataProcessProgress("Adding data", (index / (double) lifeRunnersDocumentsCount) * 100, farmId);
        }

        double currentValue = tps.at(tickInt - 1);
        double calculatedValue = currentValue + std::stod(lifeRunnerDataJSON["tickPerSecond"].dump());

        tps[tickInt - 1] = calculatedValue;
        runnersCount[tickInt - 1]++;

        creaturesCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["lifesCount"].dump());

        poopCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["poopCount"].dump());
        pheromoneCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["pheromoneCount"].dump());
        eatCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["eatCount"].dump());
        eatLifeCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["eatLifeCount"].dump());
        mateFailureCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["mateFailureCount"].dump());
        mateSuccessCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["mateSuccessCount"].dump());
        biteCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["biteCount"].dump());
        biteLifeCount[tickInt - 1] += std::stoi(lifeRunnerDataJSON["biteLifeCount"].dump());

        chunkSelection[tickInt - 1] += std::stod(lifeRunnerDataJSON["chunkSelection"].dump());
        accessibleEntities[tickInt - 1] += std::stod(lifeRunnerDataJSON["accessibleEntities"].dump());
        sensorProcessing[tickInt - 1] += std::stod(lifeRunnerDataJSON["sensorProcessing"].dump());
        brainProcessing[tickInt - 1] += std::stod(lifeRunnerDataJSON["brainProcessing"].dump());
        externalActions[tickInt - 1] += std::stod(lifeRunnerDataJSON["externalActions"].dump());
        moveCreatures[tickInt - 1] += std::stod(lifeRunnerDataJSON["moveCreatures"].dump());
        tickTime[tickInt - 1] += std::stod(lifeRunnerDataJSON["tickTime"].dump());


        index++;
    }


    auto tpsJSONArray = bsoncxx::builder::basic::array{};
    auto runnersCountJSONArray = bsoncxx::builder::basic::array{};
    auto creaturesCountJSONArray = bsoncxx::builder::basic::array{};
    auto poopCountJSONArray = bsoncxx::builder::basic::array{};
    auto pheromoneCountJSONArray = bsoncxx::builder::basic::array{};
    auto eatCountJSONArray = bsoncxx::builder::basic::array{};
    auto eatLifeCountJSONArray = bsoncxx::builder::basic::array{};
    auto mateFailureCountJSONArray = bsoncxx::builder::basic::array{};
    auto mateSuccessCountJSONArray = bsoncxx::builder::basic::array{};
    auto biteCountJSONArray = bsoncxx::builder::basic::array{};
    auto biteLifeCountJSONArray = bsoncxx::builder::basic::array{};
    auto chunkSelectionJSONArray = bsoncxx::builder::basic::array{};
    auto accessibleEntitiesJSONArray = bsoncxx::builder::basic::array{};
    auto sensorProcessingJSONArray = bsoncxx::builder::basic::array{};
    auto brainProcessingJSONArray = bsoncxx::builder::basic::array{};
    auto externalActionsJSONArray = bsoncxx::builder::basic::array{};
    auto moveCreaturesJSONArray = bsoncxx::builder::basic::array{};
    auto tickTimeJSONArray = bsoncxx::builder::basic::array{};

    auto array_builder = bsoncxx::builder::basic::array{};


    for (int it = 0; it < tickCount; it++) {

        if (index % 100 == 0) {
            saveFarmDataProcessProgress("Creating json", (it / (double) tickCount) * 100, farmId);
        }


        tps[it] = tps[it] / ((double) runnersCount[it]);

        tpsJSONArray.append(tps.at(it));
        runnersCountJSONArray.append(runnersCount.at(it));
        creaturesCountJSONArray.append(creaturesCount.at(it));
        poopCountJSONArray.append(poopCount.at(it));
        pheromoneCountJSONArray.append(pheromoneCount.at(it));
        eatCountJSONArray.append(eatCount.at(it));
        eatLifeCountJSONArray.append(eatLifeCount.at(it));
        mateFailureCountJSONArray.append(mateFailureCount.at(it));
        mateSuccessCountJSONArray.append(mateSuccessCount.at(it));
        biteCountJSONArray.append(biteCount.at(it));
        biteLifeCountJSONArray.append(biteLifeCount.at(it));
        chunkSelectionJSONArray.append(chunkSelection.at(it));
        accessibleEntitiesJSONArray.append(accessibleEntities.at(it));
        sensorProcessingJSONArray.append(sensorProcessing.at(it));
        brainProcessingJSONArray.append(brainProcessing.at(it));
        externalActionsJSONArray.append(externalActions.at(it));
        moveCreaturesJSONArray.append(moveCreatures.at(it));
        tickTimeJSONArray.append(tickTime.at(it));
    }



    auto builder = bsoncxx::builder::stream::document{};
    builder << "$set" << bsoncxx::builder::stream::open_document
            << "tps" << tpsJSONArray
            << "runnersCount" << runnersCountJSONArray
            << "creaturesCount" << creaturesCountJSONArray
            << "actions" << bsoncxx::builder::stream::open_document
            << "poopCount" << poopCountJSONArray
            << "pheromoneCount" << pheromoneCountJSONArray
            << "eatCount" << eatCountJSONArray
            << "eatLifeCount" << eatLifeCountJSONArray
            << "mateFailureCount" << mateFailureCountJSONArray
            << "mateSuccessCount" << mateSuccessCountJSONArray
            << "biteCount" << biteCountJSONArray
            << "biteLifeCount" << biteLifeCountJSONArray
            << close_document
            << "times" << open_document
            << "chunkSelection" << chunkSelectionJSONArray
            << "accessibleEntities" << accessibleEntitiesJSONArray
            << "sensorProcessing" << sensorProcessingJSONArray
            << "brainProcessing" << brainProcessingJSONArray
            << "externalActions" << externalActionsJSONArray
            << "moveCreatures" << moveCreaturesJSONArray
            << "tickTime" << tickTimeJSONArray
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::close_document;



    saveFarmDataProcessProgress("Saving", 0, farmId);
    bsoncxx::document::value doc_value = builder  << bsoncxx::builder::stream::finalize;

    saveFarmDataProcessProgress("Finished", 100, farmId);

    mongoClient->updateFarm(doc_value.view(), farmId);
}