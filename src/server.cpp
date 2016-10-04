#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

template <class T>
SocketServer<T>::SocketServer(const int port) : _socket_functions_helper(port)
{

}

template <class T>
SocketServer<T>::~SocketServer()
{

}

template <class T>
void SocketServer<T>::StartServer()
{
    int server_sock = _socket_functions_helper.CreateListenerSocket();
    if (server_sock < 0) {
        std::cerr << "setup of listening socket failed" << std::endl;
        return;
    }
    while (true) {
        std::cout << "Waiting for a new client connection..." << std::endl;
        int client_sock = _socket_functions_helper.WaitForClientConnection(server_sock);
        if (client_sock < 0) {
            std::cout << "accepted socket was bad!" << std::endl;
            return;
        }

        bool found_space = false;

        while (!found_space) {
            char buffer[256];
            int n = _socket_functions_helper.WaitForData(client_sock, buffer, 255);

            if (n == 0) {
                // client closed connection
                std::cout << "client closed connection!" << std::endl;
                break;
            }
            if (n < 0) {
                std::cout << "n less than 0" << std::endl;
                break;
            }
            printf("message is %s", buffer);

            std::cout << std::endl;
            for (int idx = 0; idx < n; ++idx) {
                //std::cout << "looking at char " << buffer[idx] << std::endl;
                if (buffer[idx] == ' ') {
                    std::cout << "found space!" << std::endl;
                    found_space = true;
                }
            }
            /*
               n = send(client_sock, "i got it", 8, MSG_NOSIGNAL);

               if (n < 0) {
               std::cout << "failed on write" << std::endl;
               return;
               }
               */

        }
    }
}
