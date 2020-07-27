#ifndef __TCP_GENERIC_SERVER_HPP__
#define __TCP_GENERIC_SERVER_HPP__


#include <iostream>
#include <stdio.h>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>


template<typename TcpHandler>
class TcpGenericServer
{
private:
    boost::asio::io_service &_io_service;

public: 
    TcpGenericServer(boost::asio::io_service & io_service)
        : _io_service(io_service) 
    {

    }
    ~TcpGenericServer() {}
public:
    void tcp_listen(uint16_t port)
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        auto acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(_io_service);
        acceptor->open(endpoint.protocol());
        acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor->bind(endpoint);
        acceptor->listen();

        auto handler = std::make_shared<TcpHandler>(_io_service);
        
        acceptor->async_accept( handler->socket(), 
                                [=](const boost::system::error_code & error_code) {
                                    handle_new_connection(acceptor, handler, error_code);
                                });
        

    }
    void handle_new_connection( std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor, 
                                std::shared_ptr<TcpHandler>                     handler, 
                                const boost::system::error_code &               error_code)
    {
        if(error_code)
        {
            std::cerr << "handler_new_connection error..: " << error_code.message() << std::endl;
        }
        else
        {
            std::cout << "connection" << std::endl;
            handler->start();
        }

        auto new_handler = std::make_shared<TcpHandler> (_io_service);

        acceptor->async_accept( new_handler->socket(), 
                                [=](boost::system::error_code error_code) {
                                    handle_new_connection(acceptor, new_handler, error_code);
                                });
        
    }
};

#endif