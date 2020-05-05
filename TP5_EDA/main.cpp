#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "AsyncDaytimeServer.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context io_context;

        AsyncDaytimeServer s(io_context);
        s.start();
        io_context.run();

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}