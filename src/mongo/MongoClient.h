//
// Created by Amalric Lombard de Buffieres on 03/07/2021.
//

#ifndef CREATURES_MONGOCLIENT_H
#define CREATURES_MONGOCLIENT_H


#include <nlohmann/json.hpp>
#include "mongocxx/client.hpp"
#include "mongocxx/uri.hpp"

class MongoClient {
private:
    mongocxx::client client;
    mongocxx::database db;

    mongocxx::collection farms;
    mongocxx::collection creatures;
    mongocxx::collection evolutions;
    mongocxx::collection genomes;
    mongocxx::collection lifeRunners;

public:
    MongoClient();

    void saveFarm(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);
    std::string saveLife(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);

    void saveEvolution(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);

    void saveGenome(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);

    void updateLife(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc, int farmId, int creatureId);

    nlohmann::json fetchFarms();

    nlohmann::json fetchFarm(std::string farmId);

    nlohmann::json fetchAllCreaturesFromFarm(std::string farmId);

    void saveLifeRunnerData(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);

    void processLifeRunnersData(std::string farmId);

    void updateFarm(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc, int farmId);


    void saveFarmDataProcessProgress(std::string step, double progressionPercentage, int farmId);

    nlohmann::json fetchAllRunnersFromFarm(int farmId);

    const mongocxx::collection &getLifeRunners() const;
};


#endif //CREATURES_MONGOCLIENT_H
