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
        bool is_word_a_shift_number = true;

        while (!found_space) {
            char buffer[256];
            ssize_t num_bytes = socket_wrapper.WaitForData(client_sock, buffer, 255);

            if (num_bytes <= 0) {
                std::cout << ((num_bytes == 0) ? "client closed connection!" : "error waiting for data!") << std::endl;
                break;
            }

            printf("message is %s\n", buffer);

            //
            // message format...
            //      shift_int message
            //
            for (int char_idx = 0; char_idx < num_bytes; ++char_idx) {
                if (buffer[char_idx] == ' ') {
                    std::cout << "found space!" << std::endl;
                    found_space = true;

                    if (is_word_a_shift_number) {
                        char* first_non_numeric_char;
                        long int shift = strtol(buffer, &first_non_numeric_char, 10);
                        std::cout << "got shift of " << shift << std::endl;
                        // we've verified there is a space,
                        // so it should be the first non numeric char if all went well
                        if (*first_non_numeric_char != ' ') {
                            socket_wrapper.CloseConnection(client_sock);
                            break;
                        }
                        is_word_a_shift_number = false;
                    } else {
                        // message
                        char response[] = "abc";
                        socket_wrapper.SendData(client_sock, response, 3);
                        break;
                    }
                }
            }
        }
    }
    return true;
}

