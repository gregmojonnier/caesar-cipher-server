#pragma once
#include <cstddef>
#include <sys/types.h>

class SocketFunctionsHelper
{
    public:
        SocketFunctionsHelper();
        virtual ~SocketFunctionsHelper();

        int CreateListenerSocket(int port) const;
        int WaitForClientConnection(int server_sock) const;
        ssize_t WaitForData(int sockfd, void* buf, std::size_t len) const;
        void CloseConnection(int sockfd) const;
        ssize_t SendData(int sockfd, char const* buf, std::size_t len) const;
};
