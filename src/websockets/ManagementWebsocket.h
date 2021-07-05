//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#ifndef CREATURES_MANAGEMENTWEBSOCKET_H
#define CREATURES_MANAGEMENTWEBSOCKET_H

#include <iostream>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "ManagementWebsocketClient.h"
#include "../mongo/MongoClient.h"
#include <string>
#include <cstdlib>
#include <functional>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

class ManagementWebsocket {
private:

    int port;
    std::vector<ManagementWebsocketClient *> clients;
    std::mutex clients_mutex;

    int totalConnection;

    MongoClient * mongoClient;

public:

    ManagementWebsocket(int port);
    void handleTread();

    void threadLoop();

    void handleClientMessage(int id, std::string message);

    void clientDisconnected(int id);

    void sendConnectionSuccessful(int id);

    void sendMessageToClient(int id, std::string message, bool lockOwner);

    void handlePing(int id, json ping);

    void handleRequest(int id, json params, json body);
};


#endif //CREATURES_MANAGEMENTWEBSOCKET_H
