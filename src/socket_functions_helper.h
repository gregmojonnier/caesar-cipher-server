#pragma once
#include <cstddef>
#include <sys/types.h>

class SocketFunctionsHelper
{
    public:
        SocketFunctionsHelper() = default;
        ~SocketFunctionsHelper() = default;

        int CreateListenerSocket(int port) const;
        int WaitForClientConnection(int server_sock) const;
        void SetReceiveDataTimeoutInSeconds(int client_sock, double timeout) const;
        ssize_t WaitForData(int sockfd, void* buf, std::size_t len) const;
        void CloseConnection(int sockfd) const;
        ssize_t SendData(int sockfd, char const* buf, std::size_t len) const;
};
