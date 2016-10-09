#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

template <class SocketFunctionsWrapper>
bool SocketServer::StartServer(int port, const SocketFunctionsWrapper& socket_wrapper)
{
    int server_sock = socket_wrapper.CreateListenerSocket(port);
    if (server_sock < 0) {
        std::cerr << "setup of listening socket failed" << std::endl;
        return false;
    }
    while (true) {
        std::cout << "Waiting for a new client connection..." << std::endl;
        int client_sock = socket_wrapper.WaitForClientConnection(server_sock);
        if (client_sock < 0) {
            std::cout << "accepted socket was bad!" << std::endl;
            return false;
        }

        bool found_space = false;

        while (!found_space) {
            char buffer[256];
            int n = socket_wrapper.WaitForData(client_sock, buffer, 255);

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

            //
            // message format...
            //      shift_int message
            //
            for (int idx = 0; idx < n; ++idx) {
                if (buffer[idx] == ' ') {
                    std::cout << "found space!" << std::endl;
                    found_space = true;
                    char* first_non_numeric_char;
                    long int shift = strtol(buffer, &first_non_numeric_char, 10);
                    std::cout << "got shift of " << shift << std::endl;

                    // we've verified there is a space,
                    // so it should be the first non numeric char if all went well
                    if (*first_non_numeric_char != ' ') {
                        socket_wrapper.CloseConnection(client_sock);
                        break;
                    }
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
    return true;
}

