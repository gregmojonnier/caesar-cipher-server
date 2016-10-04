#pragma once
#include <cstddef>
class SocketFunctionsHelper
{
    public:
        SocketFunctionsHelper(const int port);
        virtual ~SocketFunctionsHelper();

        int CreateListenerSocket();
        int WaitForClientConnection(int server_sock);
        int WaitForData(int sockfd, void* buf, std::size_t len);

    private:
        const int _port;
};
