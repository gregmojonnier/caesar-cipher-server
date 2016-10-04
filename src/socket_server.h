#pragma once
#include "socket_functions_helper.h"
class SocketServer
{
    public:
        SocketServer() = default;
        virtual ~SocketServer() = default;

        template<class SocketFunctionsWrapper=SocketFunctionsHelper>
        bool StartServer(int port, const SocketFunctionsWrapper& socket_wrapper=SocketFunctionsWrapper());
};

#include "socket_server.cpp"
