#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <memory>
#include "caesar_cipher_data_interpreter.h"
#include "socket_server.h"
#include <errno.h>

bool SocketServer::StartServer(int port)
{
    int server_sock = _socket_wrapper.CreateListenerSocket(port);
    if (server_sock < 0) {
        std::cerr << "setup of listening socket failed" << std::endl;
        return false;
    }

    while (true) {
        std::cout << "Waiting for a new client connection..." << std::endl;
        int client_sock = _socket_wrapper.WaitForClientConnection(server_sock);
        if (client_sock < 0) {
            std::cout << "accepted socket was bad!" << std::endl;
            return false;
        }

        _socket_wrapper.SetReceiveDataTimeoutInSeconds(client_sock, 1.5);
        std::unique_ptr<IDataInterpreter> data_interpreter = std::make_unique<CaesarCipherDataInterpreter>();

        while (true) {
            char buffer[512];
            ssize_t num_bytes = _socket_wrapper.WaitForData(client_sock, buffer, 511);
            if (num_bytes == 0) {
                std::cout << ((num_bytes == 0) ? "client closed connection!" : "error waiting for data!") << std::endl;
                break;
            } else if (num_bytes == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    std::cout << "Timed out the max number of times, closing connection!" << std::endl;
                    _socket_wrapper.CloseConnection(client_sock);
                    break;
                }
                std::cerr << "An error on reading data." << std::endl;
                break;
            }

            printf("message is %s\n", buffer);
            std::vector<std::string> complete_words = data_interpreter->SplitDataIntoCompleteWords(buffer, num_bytes);
            try {
                std::string response = data_interpreter->InterpretWordsAndDetermineResponse(complete_words);
                if (!response.empty()) {
                    _socket_wrapper.SendData(client_sock, response.c_str(), response.size());
                }
            } catch(const InvalidShiftException& e) {
                std::cout << e.what();
                _socket_wrapper.CloseConnection(client_sock);
                break;
            }
        }
    }
    return true;
}
