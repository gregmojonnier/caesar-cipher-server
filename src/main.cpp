#include "socket_server.h"
int main(int argc, char *argv[])
{
    SocketServer server;
    server.StartServer(55555);
    return 0;
}
