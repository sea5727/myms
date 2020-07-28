#include <iostream>
#include "EventHandler.hpp"

int main()
{
    std::cout << "Hello world\n" ;
    auto a = EventHandler::Instance();

    auto f = EventHandler::_handler_table_t[0];
    // auto f = EventHandler::_handler_table_t;
    // message_1_t aa;
    // f(aa);
}