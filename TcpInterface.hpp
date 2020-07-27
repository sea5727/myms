#ifndef __TCP_INTERFACE_HPP__
#define __TCP_INTERFACE_HPP__


#include <iostream>
#include <boost/asio.hpp>

class TcpInterface : public std::enable_shared_from_this<TcpInterface>
{
private:
    char buffer[8092];
    boost::asio::io_service & _io_service; 
    boost::asio::ip::tcp::socket _socket;
    boost::asio::io_service::strand _strand;
public:
    TcpInterface(boost::asio::io_service & io_service)
        : _io_service(io_service)
        , _socket(io_service)
        , _strand(io_service)
    {

    }
    ~TcpInterface() {}

    boost::asio::ip::tcp::socket & socket()
    {
        return _socket;
    }
    void start()
    {
        read_packet();
    }
    void read_packet()
    {
        memset(buffer, 0x00, sizeof(buffer));
        boost::asio::async_read(_socket,
                                boost::asio::buffer(buffer),
                                boost::asio::transfer_at_least(1),
                                [me=shared_from_this()](const boost::system::error_code & error_code, std::size_t read_size){
                                    me->read_packet_done(error_code, read_size);
                                });
    }
    void read_packet_done(const boost::system::error_code & error_code, std::size_t read_size)
    {
        if(error_code)
        {
            std::cerr << "read_packet_done error.. : " << error_code.message() << std::endl;
            try
            {
                _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
                _socket.close();
            }
            catch(std::exception const &e)
            {
                std::cerr << "catch.. e.what : " << e.what() << std::endl;
            }
            catch(...)
            {
                std::cerr << "catch oops.." << std::endl;
            }
            return;
        }
        std::cout << buffer << std::endl;
        read_packet();
    }
};


#endif