#pragma once
class SocketFunctionsHelper
{
    public:
        SocketFunctionsHelper(const int port);
        virtual ~SocketFunctionsHelper();

        int CreateListenerSocket();
        int WaitForClientConnection(int server_sock);

    private:
        const int _port;
};
