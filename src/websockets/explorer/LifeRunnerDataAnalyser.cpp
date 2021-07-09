//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#include "LifeRunnerDataAnalyser.h"
#include "../../farm/statistics/ProcessedStatisticSeries.h"

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


std::vector<ProcessedStatisticSeries *> LifeRunnerDataAnalyser::processCreatures() {
    saveFarmDataProcessProgress("Fetching creatures", 0, farmId);

    auto creatures = mongoClient->fetchAllCreaturesFromFarm(std::to_string(farmId));
    auto creaturesCount = mongoClient->countCreaturesFromFarm(farmId);

    std::vector<double> births;
    std::vector<double> deaths;
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

    std::vector<ProcessedStatisticSeries *> series;

    series.emplace_back(new ProcessedStatisticSeries(farmId, "deaths", deaths));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "births", births));




    return series;
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
    std::vector<double> runnersCount;
    std::vector<double> creaturesCount;

    std::vector<double> poopCount;
    std::vector<double> pheromoneCount;
    std::vector<double> eatCount;
    std::vector<double> eatLifeCount;
    std::vector<double> mateFailureCount;
    std::vector<double> mateSuccessCount;
    std::vector<double> biteCount;
    std::vector<double> biteLifeCount;

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

    saveFarmDataProcessProgress("Creating series", 0, farmId);


    std::vector<ProcessedStatisticSeries *> series;

    series.emplace_back(new ProcessedStatisticSeries(farmId, "tps", tps));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "creaturesCount", creaturesCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "runnersCount", runnersCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "poopCount", poopCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "pheromoneCount", pheromoneCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "eatCount", eatCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "eatLifeCount", eatLifeCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "mateFailureCount", mateFailureCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "mateSuccessCount", mateSuccessCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "biteCount", biteCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "biteLifeCount", biteLifeCount));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "chunkSelection", chunkSelection));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "accessibleEntities", accessibleEntities));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "sensorProcessing", sensorProcessing));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "brainProcessing", brainProcessing));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "externalActions", externalActions));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "moveCreatures", moveCreatures));
    series.emplace_back(new ProcessedStatisticSeries(farmId, "tickTime", tickTime));

    std::vector<ProcessedStatisticSeries *> creaturesSeries = processCreatures();
    series.insert(series.end(), creaturesSeries.begin(), creaturesSeries.end());

    for (int it = 0; it < series.size(); it++) {
        saveFarmDataProcessProgress("Processing statistics", (it / (double) series.size()) * 100, farmId);

        series.at(it)->process();
        series.at(it)->saveToMongo();
    }





    saveFarmDataProcessProgress("Finished", 100, farmId);






//    auto builder = bsoncxx::builder::stream::document{};
//    builder << "$set" << bsoncxx::builder::stream::open_document
//            << "tps" << tpsJSONArray
//            << "runnersCount" << runnersCountJSONArray
//
//    mongoClient->updateFarm(doc_value.view(), farmId);
}