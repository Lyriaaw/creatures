//
// Created by Amalric Lombard de Buffieres on 03/07/2021.
//

#include <vector>
#include <unistd.h>



#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "MongoClient.h"
#include "../farm/statistics/LifeRunnerDataTracker.h"

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

MongoClient::MongoClient() {

    client = mongocxx::client{mongocxx::uri{"mongodb://sleipnir:sleipnir_secret@127.0.0.1:27017/?authSource=sleipnir_db"}};
    db = client["sleipnir_db"];


    farms = db["creatures_farms"];
    creatures = db["creatures_creatures"];
    evolutions = db["creatures_evolutions"];
    genomes = db["creatures_genomes"];
    lifeRunners = db["creatures_life_runners"];
    statistics_series = db["creatures_statistics"];


}

void MongoClient::saveFarm(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc) {
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =  farms.insert_one(doc, {});
}

void MongoClient::updateFarm(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc, int farmId) {
    farms.update_one(bsoncxx::builder::stream::document{} << "farm_id" << farmId << bsoncxx::builder::stream::finalize, doc);
}


void MongoClient::saveStatisticsSeries(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc) {
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =  statistics_series.insert_one(doc, {});
}



nlohmann::json MongoClient::fetchFarms() {
    nlohmann::json farmsJSONArray = nlohmann::json::array();

    mongocxx::options::find opts{};
    opts.projection(document{} << "_id" << 1 << "farm_id" << 1 << finalize);

    mongocxx::cursor cursor = farms.find(
            document{} << "farm_id" << open_document <<
                       "$exists" << true
                       << close_document << finalize, opts);


    for(auto doc : cursor) {
        nlohmann::json currentFarm = nlohmann::json::parse(bsoncxx::to_json(doc));

        auto response = creatures.count_documents(document{} << "farmId" << std::stoi(currentFarm["farm_id"].dump()) << finalize);
        currentFarm["creatures_count"] = response;

        farmsJSONArray.emplace_back(currentFarm);
    }

    return farmsJSONArray;
}

int MongoClient::countCreaturesFromFarm(int farmId) {
    return creatures.count_documents(document{} << "farmId" << farmId << finalize);
}


nlohmann::json MongoClient::fetchAllCreaturesFromFarm(std::string farmId) {
    int farmIdInt = std::stoi(farmId);

    nlohmann::json creaturesJSONArray = nlohmann::json::array();

    mongocxx::cursor cursor = creatures.find(document{} << "farmId" << farmIdInt << finalize);


    for(auto doc : cursor) {
        nlohmann::json currentFarm = nlohmann::json::parse(bsoncxx::to_json(doc));

        creaturesJSONArray.emplace_back(currentFarm);
    }

    return creaturesJSONArray;
}




void MongoClient::processLifeRunnersData(std::string farmId) {
    int farmIdInt = std::stoi(farmId);



}




nlohmann::json MongoClient::fetchFarm(std::string farmId) {
    int farmIdInt = std::stoi(farmId);

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = farms.find_one(document{} << "farm_id" << farmIdInt << finalize);
    if(!maybe_result) {
        return {"error", "farm_not_found"};
    }

    nlohmann::json farmJSON = nlohmann::json::parse(bsoncxx::to_json(*maybe_result));


    auto creatures_count = creatures.count_documents(document{} << "farmId" << farmIdInt << finalize);
    farmJSON["creatures_count"] = creatures_count;

    nlohmann::json creaturesJSONArray = fetchAllCreaturesFromFarm(farmId);






    return farmJSON;
}






nlohmann::json MongoClient::fetchStatisticsSeries(std::string farmId, std::string seriesName) {
    int farmIdInt = std::stoi(farmId);

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = statistics_series.find_one(document{} << "farm_id" << farmIdInt << "type" << seriesName << finalize);
    if(!maybe_result) {
        return {"error", "farm_not_found"};
    }

    nlohmann::json seriesJSON = nlohmann::json::parse(bsoncxx::to_json(*maybe_result));


    return seriesJSON;
}






std::string MongoClient::saveLife(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc) {
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =  creatures.insert_one(doc, {});
    return result->inserted_id().get_oid().value.to_string();
}

void MongoClient::updateLife(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc, int farmId, int creatureId) {
    creatures.update_one(bsoncxx::builder::stream::document{} << "farmId" << farmId << "id" << creatureId << bsoncxx::builder::stream::finalize, doc);
}



void MongoClient::saveEvolution(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc) {
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =  evolutions.insert_one(doc, {});
}

void MongoClient::saveGenome(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc) {
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =  genomes.insert_one(doc, {});
}


void MongoClient::saveLifeRunnerData(bsoncxx::view_or_value<bsoncxx::document::view, bsoncxx::document::value> doc) {
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =  lifeRunners.insert_one(doc, {});
}


const mongocxx::collection &MongoClient::getLifeRunners() const {
    return lifeRunners;
}




//
//auto builder = bsoncxx::builder::stream::document{};
//bsoncxx::document::value doc_value = builder
//        << "name" << "MongoDB"
//        << "type" << "database"
//        << "count" << 1
//        << "versions"
//        << open_array
//        << "v3.2" << "v3.0" << "v2.6"
//        << close_array
//        << "info" << bsoncxx::builder::stream::open_document
//        << "x" << 203
//        << "y" << 102
//        << bsoncxx::builder::stream::close_document
//        << bsoncxx::builder::stream::finalize;