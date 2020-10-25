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


                clients.emplace_back(client);
                client->do_session(std::move(socket));
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





std::string WebUiConnection::initialData() {
    JSONCreator message;
    message.addJSONKeyValue("farm", getFarmObject());
    return message.getResult();
}

std::string WebUiConnection::getFarmObject() {
    JSONCreator farmJSON;
    farmJSON.addIntKeyValue("tick", farm->getTickCount());
    farmJSON.addIntKeyValue("creatures", farm->getLifes().size());
    farmJSON.addDoubleKeyValue("tick_per_second", farm->getDataAnalyser().getTickPerSecond()->getAveragedLastValue());

    JSONCreator message;
    message.addJSONKeyValue("farm", farmJSON.getResult());
    return message.getResult();
}

std::string WebUiConnection::sendEvent(std::string type, std::string JSONBody) {
    JSONCreator event;
    event.addStringKeyValue("type", type);
    event.addJSONKeyValue("body", JSONBody);



    JSONCreator dto;
    dto.addJSONKeyValue("event", event.getResult());

    return dto.getResult();
}



void WebUiConnection::updateClients() {
    std::lock_guard<std::mutex> guard(clients_mutex);

    for (auto const& client : clients) {
        std::string messageToSend = sendEvent("farm_update", getFarmObject());


        client->sendMessage(messageToSend);
    }
}
