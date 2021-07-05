//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#include "ManagementWebsocket.h"
#include "explorer/ExplorerFarms.h"

ManagementWebsocket::ManagementWebsocket(int port) : port(port), totalConnection(0), mongoClient(new MongoClient()) {
}

void ManagementWebsocket::handleTread() {
    auto f = [&]() {
        this->threadLoop();
    };

    std::thread webUiConnectionThread(f);
    webUiConnectionThread.detach();
}

void ManagementWebsocket::threadLoop() {
    try{
        auto const address = net::ip::make_address("0.0.0.0");
        auto const port = static_cast<unsigned short>(ManagementWebsocket::port);

        std::cout << "Starting management websocket on port " << ManagementWebsocket::port << std::endl;


        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {

            tcp::socket socket{ioc};

            acceptor.accept(socket);
            totalConnection++;

            auto f = [&](tcp::socket socket) {
                ManagementWebsocketClient * client = new ManagementWebsocketClient(totalConnection);

                client->setClientDisconnected([&](int id) {
                    this->clientDisconnected(id);
                });

                client->setHandleClientMessage([&](int id, std::string message) {
                    try {
                        this->handleClientMessage(id, message);
                    } catch (const std::exception& e) {
                        std::cerr << "Error while receiving message: " << e.what() << std::endl;
                    }
                });


                clients.emplace_back(client);
                client->initClient(std::move(socket));
                sendConnectionSuccessful(client->getId());
                client->do_session();
            };

            std::thread(f,std::move(socket)).detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void ManagementWebsocket::clientDisconnected(int id) {
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




void ManagementWebsocket::handleClientMessage(int id, std::string message) {
    std::lock_guard<std::mutex> guard(clients_mutex);

    ManagementWebsocketClient * client = nullptr;

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

    if (j["type"] == "ping") {
        handlePing(id, j["body"]);
    } else if (j["type"] == "request") {
        handleRequest(id, j["params"], j["body"]);
//    } else if (j["type"] == "farm_control") {
//        handleNewFarmControl(id, j);
//    } else if (j["type"] == "ping") {
    } else {
        std::cout << "Unrecognized messsage from client id " << id << " => " << message << std::endl;
    }
}

void ManagementWebsocket::handleRequest(int id, json params, json body) {
    auto * explorerFarm = new ExplorerFarms(mongoClient, params, body);
    sendMessageToClient(id, explorerFarm->handleRequest().dump(), true);
}





void ManagementWebsocket::sendConnectionSuccessful(int id) {
    json initialData = {
            {"type", "connection_successful"},
    };
//
//
    sendMessageToClient(id, initialData.dump(), false);
////    sendMessageToClient(id, sendEvent("farm_control_update", farm->getFarmControl()->asJSON()).dump(), false);
}

void ManagementWebsocket::sendMessageToClient(int id, std::string message, bool lockOwner) {
    if (!lockOwner) {
        std::lock_guard<std::mutex> guard(clients_mutex);
    }

    ManagementWebsocketClient * client = nullptr;

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


void ManagementWebsocket::handlePing(int id, json ping) {


//
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    double statisticsTime = end.time_since_epoch().count();

//    std::cout << "Received ping: " << ping.dump() << std::endl;

    json pingData = {
            {"sent_at", ping["sent_at"]},
            {"server_time", statisticsTime},
            {"type", "ping"},
    };
//

    sendMessageToClient(id, pingData.dump(), true);
};