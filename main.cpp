#include <iostream>


#include <array>
#include <ctime>
#include <bsoncxx/json.hpp>

#include "src/ui/MainWindow.h"
#include "src/websockets/WebUiConnection.h"



#include "mongocxx/client.hpp"
#include "mongocxx/uri.hpp"


int main(int argc, char **argv)
{
    srand((unsigned) time(0));

    MainWindow window = MainWindow();
    window.start();

//    auto client = mongocxx::client{mongocxx::uri{"mongodb://sleipnir:sleipnir_secret@127.0.0.1:27017/?authSource=sleipnir_db"}};
//    mongocxx::database db = client["sleipnir_db"];
//
//    mongocxx::collection collection = db["concept_AwsCost"];
//
//    mongocxx::cursor cursor = collection.find({});
//    for(auto doc : cursor) {
//        std::cout << bsoncxx::to_json(doc) << "\n";
//    }

    return 0;
}


