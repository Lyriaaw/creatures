//
// Created by Amalric Lombard de Buffieres on 10/04/2021.
//

#ifndef CREATURES_LIFERUNNERWEBSOCKET_H
#define CREATURES_LIFERUNNERWEBSOCKET_H
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "LifeRunnerWebsocketClient.h"

using json = nlohmann::json;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class LifeRunnerWebsocket {
private:
    int id;
    int port;
    std::vector<LifeRunnerWebsocketClient *> clients;
    std::mutex clients_mutex;

    int totalConnection;


public:
    LifeRunnerWebsocket(int id);

    void handleTread();

    void threadLoop();

    void clientDisconnected(int id);

    void handleClientMessage(int id, std::string message);

    int getPort() const;

    void sendConnectionSuccessful(int id);

    void sendMessageToClient(int id, std::string message, bool lockOwner);

    void broadcastMessage(json message);
};


#endif //CREATURES_LIFERUNNERWEBSOCKET_H
