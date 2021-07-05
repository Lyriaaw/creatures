
//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#ifndef CREATURES_MANAGEMENTWEBSOCKETCLIENT_H
#define CREATURES_MANAGEMENTWEBSOCKETCLIENT_H
#include <iostream>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <cstdlib>
#include <functional>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class ManagementWebsocketClient {
private:

    websocket::stream<tcp::socket> * ws;
    int id;

    std::function<void(int id)> clientDisconnected;
    std::function<void(int id, std::string message)> handleClientMessage;


public:
    ManagementWebsocketClient(int id);


    void initClient(tcp::socket socket);

    void do_session();

    void sendMessage(std::string message);

    void setClientDisconnected(const std::function<void(int)> &clientDisconnected);

    int getId() const;

    void setHandleClientMessage(const std::function<void(int, std::string)> &handleClientMessage);
};


#endif //CREATURES_MANAGEMENTWEBSOCKETCLIENT_H
