#ifndef __WEB_SOCKET_SERVER_HPP
#define __WEB_SOCKET_SERVER_HPP

#include <boost/bind/bind.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef server::message_ptr message_ptr;


// Define a callback to handle incoming messages
void on_message2(websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
}

void test()
{
    
}
class WebSocketServer
{
public:
    boost::asio::io_service &_io_service;
    server web_socket_server;
public:
    WebSocketServer(boost::asio::io_service & io_service)
        : _io_service(io_service)
    {

    }
    ~WebSocketServer()
    {

    }
    void listen(uint16_t port)
    {
        web_socket_server.set_access_channels(websocketpp::log::alevel::all);
        web_socket_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        web_socket_server.init_asio(&_io_service);
        
        web_socket_server.set_message_handler(boost::bind(&WebSocketServer::on_message, this, boost::placeholders::_1, boost::placeholders::_2));
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        web_socket_server.listen(endpoint);
        web_socket_server.start_accept();

    }
    void on_message(websocketpp::connection_hdl hdl, message_ptr msg)
    {
        std::cout << "on_message called with hdl: " << hdl.lock().get()
                << " and message: " << msg->get_payload()
                << std::endl;
        
        // check for a special command to instruct the server to stop listening so
        // it can be cleanly exited.
        if (msg->get_payload() == "stop-listening") {
            web_socket_server.stop_listening();
            return;
        }

        // try {
        //     web_socket_server.send(hdl, msg->get_payload(), msg->get_opcode());
        // } catch (websocketpp::exception const & e) {
        //     std::cout << "Echo failed because: "
        //             << "(" << e.what() << ")" << std::endl;
        // }
    }


};

#endif