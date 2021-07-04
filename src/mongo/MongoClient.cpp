//
// Created by Amalric Lombard de Buffieres on 03/07/2021.
//
#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "MongoClient.h"

MongoClient::MongoClient() {

    client = mongocxx::client{mongocxx::uri{"mongodb://sleipnir:sleipnir_secret@127.0.0.1:27017/?authSource=sleipnir_db"}};
    db = client["sleipnir_db"];


    farms = db["creatures_farms"];
    creatures = db["creatures_creatures"];
    evolutions = db["creatures_evolutions"];
    genomes = db["creatures_genomes"];


}

void MongoClient::saveFarm(nlohmann::json json) {
    auto doc = bsoncxx::from_json(json.dump()).view();
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =  farms.insert_one(doc, {});
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