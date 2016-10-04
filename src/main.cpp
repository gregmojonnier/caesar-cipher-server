#include "socket_server.h"
#include "socket_functions_helper.h"
int main(int argc, char *argv[])
{
    SocketServer server;
    server.StartServer(55555);
    return 0;
}
