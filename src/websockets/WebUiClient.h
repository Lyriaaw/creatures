//
// Created by Amalric Lombard de Buffières on 10/25/20.
//

#ifndef CREATURES_WEBUICLIENT_H
#define CREATURES_WEBUICLIENT_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "../farm/Farm.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebUiClient {
private:
    websocket::stream<tcp::socket> * ws;
    int id;

    std::function<void(int id)> clientDisconnected;
public:
    WebUiClient(int id);

    void do_session(tcp::socket socket);

    void sendMessage(std::string message);

    void setClientDisconnected(const std::function<void(int)> &clientDisconnected);

    int getId() const;
};


#endif //CREATURES_WEBUICLIENT_H
