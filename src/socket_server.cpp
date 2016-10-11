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

        bool process_messages = true;
        bool is_word_a_shift_number = true;

        while (process_messages) {
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
            //
            bool is_last_word_complete = false;
            std::vector<char*> words = SplitMessageIntoWordsBySpaces(buffer, is_last_word_complete);

            std::string response;
            long int shift;
            size_t last_word_idx = (!words.empty()) ? (words.size() - 1) : 0;
            for (int i = 0; i < words.size(); ++i) {
                char* word = words[i];
                if (i == last_word_idx && !is_last_word_complete) {
                    // TODO handle incomplete words
                    break;
                }

                if (is_word_a_shift_number) {
                    // shift
                    char* first_non_numeric_char;
                    shift = strtol(word, &first_non_numeric_char, 10);
                    std::cout << "got shift of " << shift << std::endl;
                    char* words_null_terminator = word + strlen(word);
                    if (first_non_numeric_char != words_null_terminator) {
                        socket_wrapper.CloseConnection(client_sock);
                        process_messages = false;
                        break;
                    }
                    is_word_a_shift_number = false;
                } else {
                    // message
                    response += _caesar_cipher.Generate(word, shift) + " ";
                    is_word_a_shift_number = true;
                    process_messages = false;
                    continue;
                }
            }

            if (!response.empty()) {
                socket_wrapper.SendData(client_sock, response.c_str(), response.size());
            }
        }
    }
    return true;
}

std::vector<char*> SocketServer::SplitMessageIntoWordsBySpaces(char* buffer, bool& is_last_word_complete)
{
    std::vector<char*> split_words;
    is_last_word_complete = false;
    bool is_buffer_invalid = buffer == NULL;
    char* full_buffer_null_terminator = (is_buffer_invalid) ? NULL : (buffer + strlen(buffer));

    if (is_buffer_invalid || buffer == full_buffer_null_terminator) {
        return split_words;
    }

    char* word;
    word = strtok(buffer," ");
    while (word != NULL)
    {
        split_words.push_back(word);
        printf ("%s\n",word);
        word = strtok(NULL, " ");
    }

    char* last_word = split_words.back();
    char* last_word_null_terminator = last_word + strlen(last_word);
    if (last_word_null_terminator != full_buffer_null_terminator) {
        is_last_word_complete = true;
    }
   
    return split_words;
}
