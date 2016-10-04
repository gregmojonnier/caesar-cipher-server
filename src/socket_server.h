#pragma once
template <class T>
class SocketServer
{
    public:
        SocketServer(const int port);
        virtual ~SocketServer();

        void StartServer();

    private:
        T _socket_functions_helper;
};

#include "socket_server.cpp"
