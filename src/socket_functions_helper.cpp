#include "socket_functions_helper.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

SocketFunctionsHelper::SocketFunctionsHelper(int port) : _port(port)
{

}

SocketFunctionsHelper::~SocketFunctionsHelper()
{

}

int SocketFunctionsHelper::CreateListenerSocket()
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        std::cerr << "failed to make socket" << std::endl;
        return sock;
    }

    struct sockaddr_in server_address;
    memset((char*)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "unable to bind" << std::endl;
        return sock;
    }

    if (listen(sock, 5) < 0) {
        std::cerr << "unable to listen" << std::endl;
        return sock;
    }

    return sock;
}


int SocketFunctionsHelper::WaitForClientConnection(int server_sock)
{
    struct sockaddr_in client_address;
    socklen_t clientlen = sizeof(client_address);

    return accept(server_sock, (struct sockaddr*)&client_address, &clientlen);
}