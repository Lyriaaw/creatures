//
// Created by Amalric Lombard de Buffieres on 05/07/2021.
//

#include "ManagementWebsocketClient.h"
#include "WebUiClient.h"

ManagementWebsocketClient::ManagementWebsocketClient(int id) : id(id) {}


void ManagementWebsocketClient::initClient(tcp::socket socket) {
    std::cout << "Client connected to explorer " << std::endl;
    ws = new websocket::stream<tcp::socket>(std::move(socket));
    ws->set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res){
                res.set(http::field::server,std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
            }));

    ws->accept();
}

void ManagementWebsocketClient::do_session(){
    try
    {

        for(;;){
            beast::flat_buffer buffer;

            ws->read(buffer);

            ws->text(ws->got_text());

            handleClientMessage(id, beast::buffers_to_string(buffer.data()));

            buffer.consume(buffer.size());
        }
    }
    catch(beast::system_error const& se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
        else {
            std::cout << "Client disconnected from explorer" << std::endl;
        }

        clientDisconnected(id);
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ManagementWebsocketClient::sendMessage(std::string message) {
    try {
        ws->write(boost::asio::buffer(message));
    } catch(beast::system_error const &se) {
        std::cerr << "Error: " << se.code().message() << std::endl;
    }
}

void ManagementWebsocketClient::setClientDisconnected(const std::function<void(int)> &clientDisconnected) {
    ManagementWebsocketClient::clientDisconnected = clientDisconnected;
}


int ManagementWebsocketClient::getId() const {
    return id;
}

void ManagementWebsocketClient::setHandleClientMessage(const std::function<void(int, std::string)> &handleClientMessage) {
    ManagementWebsocketClient::handleClientMessage = handleClientMessage;
}
