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
    } else {
        std::cout << "Unrecognized messsage from client id " << id << " => " << message << std::endl;
    }

}

void WebUiConnection::handleClientRequest(int id, json request) {
    if(request["component"] == "biomass_report") {
        std::cout << "Computing energy report for client " << id << std::endl;
        farm->fillEnergyDataAnalyser();
        std::cout << "Analysed for client " << id << std::endl;
        json report = farm->getBiomassDataTracker().getLastTickAsJson();

        std::cout << "Report: " << report.dump() << std::endl;
        sendMessageToClient(id, sendEvent("biomass_report", report).dump(), true);

        std::cout << "Sent to client " << id << std::endl;
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
    json initialData = {
        {"farm", getFarmObject()},
        {"runners", farm->getRunnersJSON()},
        {"map", farm->getMap()->asJson()},
    };


    sendMessageToClient(id, sendEvent("initial_data", initialData).dump(), false);
}



void WebUiConnection::updateFarmClients() {
    std::lock_guard<std::mutex> guard(clients_mutex);

    for (auto const& client : clients) {
        std::string messageToSend = sendEvent("farm_update", {{"farm", getFarmObject()}, {"runners", farm->getRunnersJSON()}}).dump();


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

json WebUiConnection::getFarmObject() {
    return {
        {"tick", farm->getMedianTick()},
        {"creatures", farm->getLifes().size()},
        {"tick_per_second", farm->getDataAnalyser().getTickPerSecond()->getAveragedLastValue()},
        {"uptime", farm->uptime()},
    };
}