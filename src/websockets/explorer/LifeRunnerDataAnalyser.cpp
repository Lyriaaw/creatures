//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#include "LifeRunnerDataAnalyser.h"

#include <iostream>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

LifeRunnerDataAnalyser::LifeRunnerDataAnalyser(int farmId): farmId(farmId), mongoClient(new MongoClient()), tickCount(0) {

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


std::vector<bsoncxx::document::value> LifeRunnerDataAnalyser::processCreatures() {
    saveFarmDataProcessProgress("Fetching creatures", 0, farmId);

    auto creatures = mongoClient->fetchAllCreaturesFromFarm(std::to_string(farmId));
    auto creaturesCount = mongoClient->countCreaturesFromFarm(farmId);

    std::vector<int> births;
    std::vector<int> deaths;
    std::vector<std::vector<double>> colors;

    births.resize(tickCount);
    deaths.resize(tickCount);
    colors.resize(tickCount);

    for (int it = 0; it < creatures.size(); it++) {

        if (it % 100 == 0) {
            saveFarmDataProcessProgress("Processing creatures", (it / ((double) creaturesCount)) * 100, farmId);
        }

        int birthTick = std::stoi(creatures.at(it)["birth_tick"].dump());
        int deathTick = creatures.at(it).find("death_tick") != creatures.at(it).end() ? std::stoi(creatures.at(it)["death_tick"].dump()) : (tickCount - 1);
        double hue = std::stod(creatures.at(it)["hue"].dump());

        for (int jt = birthTick; jt < deathTick; jt++) {
            colors.at(jt).emplace_back(hue);
        }

        deaths.at(deathTick) += 1;
        births.at(birthTick) += 1;

    }

    std::vector<std::map<double, int>> colorsMap;

    auto birthsBuilder = bsoncxx::builder::basic::array{};
    auto deathsBuilder = bsoncxx::builder::basic::array{};
    auto colorsBuilder = bsoncxx::builder::basic::array{};


    for (int it = 0; it < tickCount; it++) {
        if (it % 100 == 0) {
            saveFarmDataProcessProgress("Processing colors", (it / ((double) tickCount)) * 100, farmId);
        }


        std::map<double, int> tickColorMap;

        auto tickColors = colors.at(it);

        for (int jt = 0; jt < tickColors.size(); jt++) {
            double currentColor = tickColors.at(jt);

            if (!tickColorMap[currentColor]) {
                tickColorMap[currentColor] = 0;
            }
            tickColorMap[currentColor] += 1;
        }

        birthsBuilder.append(births.at(it));
        deathsBuilder.append(deaths.at(it));

        auto tickColorsBuilder = bsoncxx::builder::basic::array{};


        for (auto const& row: tickColorMap) {
            auto currentCreatureColor = bsoncxx::builder::stream::document{};

            currentCreatureColor << "color" << row.first << "count" << row.second;
            tickColorsBuilder.append(currentCreatureColor);
        }

        colorsBuilder.append(tickColorsBuilder);
    }


    std::vector<bsoncxx::document::value> documents;
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "births" << "farm_id" << farmId << "values" << birthsBuilder << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "deaths" << "farm_id" << farmId << "values" << deathsBuilder << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "colors" << "farm_id" << farmId << "values" << colorsBuilder << finalize);


    return documents;
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


    int index = 0;

    for(auto doc : cursor) {
        nlohmann::json lifeRunnerDataJSON = nlohmann::json::parse(bsoncxx::to_json(doc));

        auto tick = lifeRunnerDataJSON["tick"].dump();
        int tickInt = std::stoi(tick);

        if (index == 0) {
            tickCount = tickInt;
            tps.resize(tickInt + 1, 0);
            runnersCount.resize(tickInt + 1, 0);
            creaturesCount.resize(tickInt + 1, 0);

            poopCount.resize(tickInt + 1, 0);
            pheromoneCount.resize(tickInt + 1, 0);
            eatCount.resize(tickInt + 1, 0);
            eatLifeCount.resize(tickInt + 1, 0);
            mateFailureCount.resize(tickInt + 1, 0);
            mateSuccessCount.resize(tickInt + 1, 0);
            biteCount.resize(tickInt + 1, 0);
            biteLifeCount.resize(tickInt + 1, 0);

            chunkSelection.resize(tickInt + 1, 0);
            accessibleEntities.resize(tickInt + 1, 0);
            sensorProcessing.resize(tickInt + 1, 0);
            brainProcessing.resize(tickInt + 1, 0);
            externalActions.resize(tickInt + 1, 0);
            moveCreatures.resize(tickInt + 1, 0);
            tickTime.resize(tickInt + 1, 0);

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




    std::vector<bsoncxx::document::value> documents;
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "tps" << "farm_id" << farmId << "values" << tpsJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "runnersCount" << "farm_id" << farmId << "values" << runnersCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "creaturesCount" << "farm_id" << farmId << "values" << creaturesCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "poopCount" << "farm_id" << farmId << "values" << poopCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "pheromoneCount" << "farm_id" << farmId << "values" << pheromoneCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "eatCount" << "farm_id" << farmId << "values" << eatCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "eatLifeCount" << "farm_id" << farmId << "values" << eatLifeCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "mateFailureCount" << "farm_id" << farmId << "values" << mateFailureCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "mateSuccessCount" << "farm_id" << farmId << "values" << mateSuccessCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "biteCount" << "farm_id" << farmId << "values" << biteCountJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "biteLifeCount" << "farm_id" << farmId << "values" << biteLifeCountJSONArray << finalize);

    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "chunkSelection" << "farm_id" << farmId << "values" << chunkSelectionJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "accessibleEntities" << "farm_id" << farmId << "values" << accessibleEntitiesJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "sensorProcessing" << "farm_id" << farmId << "values" << sensorProcessingJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "brainProcessing" << "farm_id" << farmId << "values" << brainProcessingJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "externalActions" << "farm_id" << farmId << "values" << externalActionsJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "moveCreatures" << "farm_id" << farmId << "values" << moveCreaturesJSONArray << finalize);
    documents.emplace_back(bsoncxx::builder::stream::document{} << "type" << "tickTime" << "farm_id" << farmId << "values" << tickTimeJSONArray << finalize);

    auto creaturesData = processCreatures();
    documents.insert(documents.end(), creaturesData.begin(), creaturesData.end());

    for (int it = 0; it < documents.size(); it++) {
        mongoClient->saveStatisticsSeries(documents.at(it).view());
    }

    saveFarmDataProcessProgress("Finished", 100, farmId);






//    auto builder = bsoncxx::builder::stream::document{};
//    builder << "$set" << bsoncxx::builder::stream::open_document
//            << "tps" << tpsJSONArray
//            << "runnersCount" << runnersCountJSONArray
//
//    mongoClient->updateFarm(doc_value.view(), farmId);
}