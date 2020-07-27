#ifndef __EVENT_LOOP_HPP__
#define __EVENT_LOOP_HPP__


#include <iostream>
#include <stdio.h>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>


class EventLoop
{
private:
    std::vector<std::thread> _loops;
    boost::asio::io_service _io_service;
    
    std::vector<std::shared_ptr<boost::asio::ip::tcp::acceptor>> _acceptors;

public: 
    EventLoop() 
        : _io_service(1) 
    {

    }
    ~EventLoop() {}
public:
    boost::asio::io_service & io_service()
    {
        return _io_service;
    }
    void run()
    {
        _loops.emplace_back([&] {
            _io_service.run();
        });
    }
};

#endif