//
// Created by Amalric Lombard de Buffières on 10/25/20.
//
#include "WebUiClient.h"


void WebUiClient::do_session(tcp::socket socket){
    try
    {
        std::cout << "Client connected" << std::endl;
        ws = new websocket::stream<tcp::socket>(std::move(socket));
        ws->set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res){
                    res.set(http::field::server,std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
                }));

        ws->accept();


        for(;;){
            beast::flat_buffer buffer;

            ws->read(buffer);

            ws->text(ws->got_text());

            buffer.consume(buffer.size());

            ws->write(boost::asio::buffer("hello"));
        }
    }
    catch(beast::system_error const& se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
        else {
            std::cout << "Client disconnected" << std::endl;
        }

        clientDisconnected(id);
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void WebUiClient::sendMessage(std::string message) {
    ws->write(boost::asio::buffer(message));
}

void WebUiClient::setClientDisconnected(const std::function<void(int)> &clientDisconnected) {
    WebUiClient::clientDisconnected = clientDisconnected;
}

WebUiClient::WebUiClient(int id) : id(id) {}

int WebUiClient::getId() const {
    return id;
}
