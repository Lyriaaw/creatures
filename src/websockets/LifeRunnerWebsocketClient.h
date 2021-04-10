//
// Created by Amalric Lombard de Buffieres on 10/04/2021.
//

#ifndef CREATURES_LIFERUNNERWEBSOCKETCLIENT_H
#define CREATURES_LIFERUNNERWEBSOCKETCLIENT_H
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class LifeRunnerWebsocketClient {
private:
    websocket::stream<tcp::socket> * ws{};
    int lifeRunnerId;
    int id;

    std::function<void(int id)> clientDisconnected;
    std::function<void(int id, std::string message)> handleClientMessage;

public:
    LifeRunnerWebsocketClient(int lifeRunnerId, int id);

    void setClientDisconnected(const std::function<void(int)> &clientDisconnected);

    void do_session();

    void initClient(tcp::socket socket);

    void sendMessage(std::string message);

    void setHandleClientMessage(const std::function<void(int, std::string)> &handleClientMessage);

    int getId() const;
};


#endif //CREATURES_LIFERUNNERWEBSOCKETCLIENT_H
