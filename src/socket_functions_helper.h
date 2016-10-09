#pragma once
#include <cstddef>
class SocketFunctionsHelper
{
    public:
        SocketFunctionsHelper();
        virtual ~SocketFunctionsHelper();

        int CreateListenerSocket(int port) const;
        int WaitForClientConnection(int server_sock) const;
        int WaitForData(int sockfd, void* buf, std::size_t len) const;
        void CloseConnection(int sockfd) const;
};
