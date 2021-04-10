//
// Created by Amalric Lombard de Buffieres on 10/04/2021.
//

#include "LifeRunnerWebsocket.h"
#include "WebUiClient.h"

LifeRunnerWebsocket::LifeRunnerWebsocket(int id) : id(id), port(25000 + id), totalConnection(0) {
    std::cout << "Starting life runner websocket on port " << port << std::endl;
    handleTread();
}

void LifeRunnerWebsocket::handleTread() {
    auto f = [&]() {
        this->threadLoop();
    };

    std::thread webUiConnectionThread(f);
    webUiConnectionThread.detach();
}

void LifeRunnerWebsocket::threadLoop() {
    try{
        auto const address = net::ip::make_address("0.0.0.0");
        auto const port = static_cast<unsigned short>(LifeRunnerWebsocket::port);

        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {

            tcp::socket socket{ioc};

            acceptor.accept(socket);
            totalConnection++;

            auto f = [&](tcp::socket socket) {
                LifeRunnerWebsocketClient * client = new LifeRunnerWebsocketClient(this->id, totalConnection);

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


void LifeRunnerWebsocket::clientDisconnected(int id) {
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




void LifeRunnerWebsocket::handleClientMessage(int id, std::string message) {
    std::lock_guard<std::mutex> guard(clients_mutex);

    LifeRunnerWebsocketClient * client = nullptr;

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

//    if (j["type"] == "request") {
//        handleClientRequest(id, j);
//    } else if (j["type"] == "map_generator") {
//        handleNewMapGenerator(id, j);
//    } else if (j["type"] == "farm_control") {
//        handleNewFarmControl(id, j);
//    } else if (j["type"] == "ping") {
//        handlePing(id, j);
//    } else {
//        std::cout << "Unrecognized messsage from client id " << id << " => " << message << std::endl;
//    }

}

void LifeRunnerWebsocket::sendConnectionSuccessful(int id) {
    json initialData = {
            {"type", "connection_successful"},
    };
//
//
    sendMessageToClient(id, initialData.dump(), false);
////    sendMessageToClient(id, sendEvent("farm_control_update", farm->getFarmControl()->asJSON()).dump(), false);
}


void LifeRunnerWebsocket::sendMessageToClient(int id, std::string message, bool lockOwner) {
    if (!lockOwner) {
        std::lock_guard<std::mutex> guard(clients_mutex);
    }

    LifeRunnerWebsocketClient * client = nullptr;

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

void LifeRunnerWebsocket::broadcastMessage(json message){
    for (auto const& client : clients) {

        client->sendMessage(message.dump());
    }
}

int LifeRunnerWebsocket::getPort() const {
    return port;
}

