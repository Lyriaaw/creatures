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

    std::string initialData();

    std::string getFarmObject();

    void updateClients();

    std::string sendEvent(std::string type, std::string JSONBody);

    void clientDisconnected(int id);
};


#endif //CREATURES_WEBUICONNECTION_H
