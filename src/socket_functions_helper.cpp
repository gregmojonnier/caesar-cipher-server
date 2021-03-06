#include "socket_functions_helper.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <cmath>

int SocketFunctionsHelper::CreateListenerSocket(int port) const
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        std::cerr << "failed to make socket" << std::endl;
        return sock;
    }

    struct sockaddr_in server_address;
    memset(reinterpret_cast<char*>(&server_address), 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, reinterpret_cast<struct sockaddr *>(&server_address), sizeof(server_address)) < 0) {
        std::cerr << "unable to bind" << std::endl;
        return sock;
    }

    if (listen(sock, 5) < 0) {
        std::cerr << "unable to listen" << std::endl;
        return sock;
    }

    return sock;
}


int SocketFunctionsHelper::WaitForClientConnection(int server_sock) const
{
    struct sockaddr_in client_address;
    socklen_t clientlen = sizeof(client_address);

    return accept(server_sock, reinterpret_cast<struct sockaddr*>(&client_address), &clientlen);
}

void SocketFunctionsHelper::SetReceiveDataTimeoutInSeconds(int client_sock, double timeout) const
{
    double fraction, whole_number;

    fraction = std::modf(timeout, &whole_number);

    struct timeval tv;
    tv.tv_sec = whole_number;
    tv.tv_usec = fraction * 1000000; // conv to microseconds

    setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, &tv,sizeof(struct timeval));
}

ssize_t SocketFunctionsHelper::WaitForData(int sockfd, void* buf, std::size_t len) const
{
    return recv(sockfd, buf, len, 0);
}
void SocketFunctionsHelper::CloseConnection(int sockfd) const
{
    shutdown(sockfd, SHUT_RDWR);
}

ssize_t SocketFunctionsHelper::SendData(int sockfd, char const* buf, std::size_t len) const
{
    return send(sockfd, buf, len, MSG_NOSIGNAL);
}
