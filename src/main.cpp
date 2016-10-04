#include "server.h"
#include "socket_functions_helper.h"
int main(int argc, char *argv[])
{
    std::cout << "hi" << std::endl;
    SocketServer<SocketFunctionsHelper> s(55555);
    return 0;
}
