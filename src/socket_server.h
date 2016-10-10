#pragma once
#include "socket_functions_helper.h"
#include "caesar_cipher.h"
#include <vector>
class SocketServer
{
    public:
        SocketServer() = default;
        virtual ~SocketServer() = default;

        template<class SocketFunctionsWrapper=SocketFunctionsHelper>
        bool StartServer(int port, const SocketFunctionsWrapper& socket_wrapper=SocketFunctionsWrapper());
    private:
        std::vector<char*> SplitMessageIntoWordsBySpaces(char* buffer, bool& is_last_word_complete);
        CaesarCipher _caesar_cipher;
};

#include "socket_server.cpp"
