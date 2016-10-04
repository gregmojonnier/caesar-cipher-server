#pragma once
#include <cstddef>
class MockSocketFunctionsHelper
{
    public:
        //SocketFunctionsHelper(const int port);
        //virtual ~SocketFunctionsHelper();

        //int CreateListenerSocket();
        MOCK_CONST_METHOD1(CreateListenerSocket, int(int port));
        //int WaitForClientConnection(int server_sock);
        MOCK_CONST_METHOD1(WaitForClientConnection, int(int server_sock));
        //int WaitForData(int sockfd, void* buf, std::size_t len);
        MOCK_CONST_METHOD3(WaitForData, int(int, void* buf, std::size_t len)); 
};
