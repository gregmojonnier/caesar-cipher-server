#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void StartServer();
int SetUpListenerSocket();
int WaitForClientConnection(int server_sock);

int main(int argc, char *argv[])
{
    StartServer();
    return 0;
}


void StartServer() {
    int server_sock = SetUpListenerSocket();
    if (server_sock < 0) {
        std::cerr << "setup of listening socket failed" << std::endl;
        return;
    }
    while (true) {
        std::cout << "Waiting for a new client connection..." << std::endl;
        int client_sock = WaitForClientConnection(server_sock);
        if (client_sock < 0) {
            std::cout << "accepted socket was bad!" << std::endl;
            return;
        }

        bool found_space = false;

        while (!found_space) {
            char buffer[256];
            int n = recv(client_sock, buffer, 255, 0);

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

int port = 55555;
int SetUpListenerSocket() {
    int sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        std::cerr << "failed to make socket" << std::endl;
        return sock;
    }

    struct sockaddr_in server_address;
    memset((char*)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
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


int WaitForClientConnection(int server_sock) {
    struct sockaddr_in client_address;
    socklen_t clientlen = sizeof(client_address);

    return accept(server_sock, (struct sockaddr*)&client_address, &clientlen);
}
