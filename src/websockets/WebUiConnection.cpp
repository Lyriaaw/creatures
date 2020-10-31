//
// Created by Amalric Lombard de Buffières on 10/25/20.
//

#include "WebUiConnection.h"
#include "JSONCreator.h"

WebUiConnection::WebUiConnection(Farm *farm) : farm(farm), totalConnection(0) {}

void WebUiConnection::handleTread() {
    auto f = [&]() {
        this->threadLoop();
    };

    std::thread webUiConnectionThread(f);
    webUiConnectionThread.detach();
}

void WebUiConnection::threadLoop() {
    try{
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(25565);

        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {

            tcp::socket socket{ioc};

            acceptor.accept(socket);
            std::cout << "Received ..." << std::endl;
            totalConnection++;

            auto f = [&](tcp::socket socket) {
                WebUiClient * client = new WebUiClient(totalConnection);

                client->setClientDisconnected([&](int id) {
                    this->clientDisconnected(id);
                });

                client->setHandleClientMessage([&](int id, std::string message) {
                   this->handleClientMessage(id, message);
                });


                clients.emplace_back(client);
                client->initClient(std::move(socket));
                sendInitialData(client->getId());
                sendInitialCreatures(client->getId());
                client->do_session();
            };

            std::thread(f,std::move(socket)).detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void WebUiConnection::clientDisconnected(int id) {
    std::lock_guard<std::mutex> guard(clients_mutex);

    int indexToDelete = -1;

    for (int it = 0; it < clients.size(); it++) {
        if (clients.at(it)->getId() == id) {
            indexToDelete = it;
            it = clients.size();
        }
    }

    if (indexToDelete == -1) {
        std::cout << "Error while trying to delete client on disconnect " << id << std::endl;
        return;
    }

    clients.erase(clients.begin() + indexToDelete);
}




void WebUiConnection::handleClientMessage(int id, std::string message) {
    std::lock_guard<std::mutex> guard(clients_mutex);

    WebUiClient * client = nullptr;

    for (int it = 0; it < clients.size(); it++) {
        if (clients.at(it)->getId() == id) {
            client = clients.at(it);
            it = clients.size();
        }
    }

    if (client == nullptr) {
        std::cout << "Error while trying to handle client message. Id " << id << " not found" << std::endl;
        return;
    }

    json j = json::parse(message);

    if (j["type"] == "request") {
        handleClientRequest(id, j);
    } else if (j["type"] == "map_generator") {
        handleNewMapGenerator(id, j);
    } else {
        std::cout << "Unrecognized messsage from client id " << id << " => " << message << std::endl;
    }

}

void WebUiConnection::handleNewMapGenerator(int id, json newGeneratorJSON) {
    MapGeneratorControl * currentGeneratorControl = farm->getMap()->getMapGeneratorControl();

    std::cout << "Raw: " << newGeneratorJSON.dump() << " - " << newGeneratorJSON["body"]["seed"].dump() << std::endl;

    if (newGeneratorJSON["body"]["seed"].dump() == "null") {
        return;
    }

    int seedInt = -1;
    float positionRatioFloat = -1;
    float zFloat = -1;
    float waterLevel = -1;
    float layers = -1;
    try {
        seedInt = std::stoi(newGeneratorJSON["body"]["seed"].dump());
        positionRatioFloat = std::stof(newGeneratorJSON["body"]["positionRatio"].dump());
        zFloat = std::stof(newGeneratorJSON["body"]["zOrdinate"].dump());
        waterLevel = std::stof(newGeneratorJSON["body"]["waterLevel"].dump());
        layers = std::stof(newGeneratorJSON["body"]["layers"].dump());
    } catch (std::exception exception) {
        return;
    }



    currentGeneratorControl->setSeed(seedInt);
    currentGeneratorControl->setPositionRatio(positionRatioFloat);
    currentGeneratorControl->setZOrdinate(zFloat);
    currentGeneratorControl->setWaterLevel(waterLevel);
    currentGeneratorControl->setLayers(layers);
    farm->getMap()->generateRandomTerrain(true);


    json mapGeneratorControlBody = {
        {"mapGenerator", farm->getMap()->getMapGeneratorControl()->asJson()}
    };

    sendMessageToClient(id, sendEvent("map_generator_control_updated", mapGeneratorControlBody).dump(), true);
};

void WebUiConnection::handleClientRequest(int id, json request) {
    if(request["component"] == "biomass_report") {
        farm->fillEnergyDataAnalyser();
        json report = farm->getBiomassDataTracker().getLastTickAsJson();
        sendMessageToClient(id, sendEvent("biomass_report", report).dump(), true);
        return;
    }

    if(request["component"] == "new_runner") {
        farm->fillEnergyDataAnalyser();
        json report = farm->getBiomassDataTracker().getLastTickAsJson();
        sendMessageToClient(id, sendEvent("biomass_report", report).dump(), true);
    }


}



void WebUiConnection::sendMessageToClient(int id, std::string message, bool lockOwner) {
    if (!lockOwner) {
        std::lock_guard<std::mutex> guard(clients_mutex);
    }

    WebUiClient * client = nullptr;

    for (int it = 0; it < clients.size(); it++) {
        if (clients.at(it)->getId() == id) {
            client = clients.at(it);
            it = clients.size();
        }
    }

    if (client == nullptr) {
        std::cout << "Error while trying to handle client message. Id " << id << " not found" << std::endl;
        return;
    }


    client->sendMessage(message);
}


json WebUiConnection::sendEvent(std::string type, json JSONBody) {
    return {{
        "event", {
            {"type", type},
            {"body", JSONBody}
        }
    }};
}


void WebUiConnection::sendInitialData(int id) {
    nlohmann::json lifesJSON;

    json initialData = {
        {"farm", getFarmObject()},
        {"runners", farm->getRunnersJSON()},
        {"map", farm->getMap()->asJson()},
        {"mapGenerator", farm->getMap()->getMapGeneratorControl()->asJson()},
    };


    sendMessageToClient(id, sendEvent("initial_data", initialData).dump(), false);
}

void WebUiConnection::sendInitialCreatures(int id) {
    nlohmann::json lifesJSON;
    std::vector<Life *> intialLifes = farm->getLifes();
    for (int it = 0; it < intialLifes.size(); it++) {
        lifesJSON[it] = intialLifes.at(it)->initialDataJson();
    }


    json initialData = {
        {"lifes", lifesJSON},
    };


    sendMessageToClient(id, sendEvent("new_creatures", initialData).dump(), false);
}

void WebUiConnection::sendNewCreatures(std::vector<Life *> creatures) {
    nlohmann::json lifesJSON;
    std::vector<Life *> intialLifes = farm->getLifes();
    for (int it = 0; it < creatures.size(); it++) {
        lifesJSON[it] = creatures.at(it)->initialDataJson();
    }


    json initialData = {
        {"lifes", lifesJSON},
    };

    for (auto const& client : clients) {
        std::string messageToSend = sendEvent("new_creatures", initialData).dump();


        client->sendMessage(messageToSend);
    }
}



void WebUiConnection::updateFarmClients() {
    std::lock_guard<std::mutex> guard(clients_mutex);

    for (auto const& client : clients) {
        std::string messageToSend = sendEvent("farm_update", {{"farm", getFarmObject()}}).dump();


        client->sendMessage(messageToSend);
    }
}
void WebUiConnection::updateMapClients() {
    std::lock_guard<std::mutex> guard(clients_mutex);

    for (auto const& client : clients) {
        std::string messageToSend = sendEvent("map_update", {{"map", farm->getMap()->asJson()}}).dump();


        client->sendMessage(messageToSend);
    }
}

void WebUiConnection::updateRunnerClients(int id) {
    LifesRunner * foundRunner = nullptr;

    for (auto const& runner: farm->getLifesRunners()) {
        if (runner->getId() == id) {
            foundRunner = runner;
            break;
        }
    }

    if (foundRunner == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> guard(clients_mutex);

    for (auto const& client : clients) {
        std::string messageToSend = sendEvent("runner_update", {{"runner", foundRunner->asJson()}}).dump();


        client->sendMessage(messageToSend);
    }
}

void WebUiConnection::updateCreaturesRunnerClients(int id) {
    LifesRunner * foundRunner = nullptr;

    for (auto const& runner: farm->getLifesRunners()) {
        if (runner->getId() == id) {
            foundRunner = runner;
            break;
        }
    }

    if (foundRunner == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> guard(clients_mutex);

    for (auto const& client : clients) {
        std::string messageToSend = sendEvent("creatures_list_updates", {{"runner", foundRunner->creaturesAsJson()}}).dump();


        client->sendMessage(messageToSend);
    }
}

void WebUiConnection::updateBiomassReportClients() {
    std::lock_guard<std::mutex> guard(clients_mutex);

    for (auto const& client : clients) {
        json report = farm->getBiomassDataTracker().getLastTickAsJson();

        std::string messageToSend = sendEvent("biomass_report", report).dump();


        client->sendMessage(messageToSend);
    }
}



json WebUiConnection::getFarmObject() {
    return {
        {"tick", farm->getMedianTick()},
        {"creatures", farm->getLifes().size()},
        {"tick_per_second", farm->getDataAnalyser().getTickPerSecond()->getAveragedLastValue()},
        {"uptime", farm->uptime()},
    };
}