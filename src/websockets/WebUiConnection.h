//
// Created by Amalric Lombard de Buffières on 10/25/20.
//

#ifndef CREATURES_WEBUICONNECTION_H
#define CREATURES_WEBUICONNECTION_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "../farm/Farm.h"
#include "WebUiClient.h"

#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebUiConnection {
private:
    Farm * farm;

    std::vector<WebUiClient *> clients;
    std::mutex clients_mutex;

    int totalConnection;

public:
    explicit WebUiConnection(Farm *farm);

    void handleTread();

    void threadLoop();

    json getFarmObject();

    void updateFarmClients();
    void updateMapClients();

    json sendEvent(std::string type, json JSONBody);

    void clientDisconnected(int id);

    void handleClientMessage(int id, std::string message);

    void sendInitialData(int id);
    void sendMessageToClient(int id, std::string message, bool lockOwner);

    void updateRunnerClients(int id);

    void handleClientRequest(int id, json request);

};


#endif //CREATURES_WEBUICONNECTION_H
