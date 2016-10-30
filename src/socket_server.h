#pragma once
#include "socket_functions_helper.h"
class SocketServer
{
    public:
        SocketServer() = default;
        ~SocketServer() = default;

        bool StartServer(int port);

    private:
        SocketFunctionsHelper _socket_wrapper;
};
