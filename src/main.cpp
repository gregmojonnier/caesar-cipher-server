#include "server.h"
#include "socket_functions_helper.h"
int main(int argc, char *argv[])
{
    SocketServer<SocketFunctionsHelper> s(55555);
    s.StartServer();
    return 0;
}
