#pragma once
#include <cstddef>
#include <string.h>
class MockSocketFunctionsHelper
{
    public:

        //int CreateListenerSocket();
        MOCK_CONST_METHOD1(CreateListenerSocket, int(int port));
        //int WaitForClientConnection(int server_sock);
        MOCK_CONST_METHOD1(WaitForClientConnection, int(int server_sock));
        //int WaitForData(int sockfd, void* buf, std::size_t len);
        MOCK_CONST_METHOD3(WaitForData, int(int, void* buf, std::size_t len)); 
};

// Needed a custom action because SetArgPointee won't allow setting a void*, which happens to be apart of the socket interface
ACTION_P(SetIncomingClientData, param) {
    memcpy(arg1, param, (size_t)sizeof param); 
}
