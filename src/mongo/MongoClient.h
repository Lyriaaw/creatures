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

public:
    MongoClient();

    void saveFarm(nlohmann::json json);
    std::string saveLife(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);

    void saveEvolution(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);

    void saveGenome(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc);

    void updateLife(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc, int farmId, int creatureId);
};


#endif //CREATURES_MONGOCLIENT_H
