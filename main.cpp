#include <iostream>
#include "EventLoop.hpp"
#include "TcpGenericServer.hpp"
#include "TcpInterface.hpp"
#include "WebSocketServer.hpp"
#include "EventHandler.hpp"
#include "MessageType.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Hello, world!\n";

    gst_init(&argc, &argv);

    EventLoop loop;
    TcpGenericServer<TcpInterface> tcp_server(loop.io_service());
    WebSocketServer web_socket_server(loop.io_service());

    tcp_server.tcp_listen(12345);
    web_socket_server.listen(9002);

    loop.run();

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }    
}
