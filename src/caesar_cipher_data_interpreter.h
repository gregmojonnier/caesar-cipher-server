#pragma once
#include "idata_interpreter.h"
#include "caesar_cipher.h"
#include <stdexcept>

class CaesarCipherDataInterpreter : public IDataInterpreter {
    public:
        CaesarCipherDataInterpreter() {_is_shift = true;}
        ~CaesarCipherDataInterpreter() = default;

        std::vector<std::string> SplitDataIntoCompleteWords(char* buffer, std::size_t num_bytes) override;
        std::string InterpretWordsAndDetermineResponse(const std::vector<std::string>& complete_words) override;

    private:
        bool _is_shift;
        int _shift;
        std::string _partial_word;
        CaesarCipher _caesar_cipher;
};

 
 
class InvalidShiftException: public std::invalid_argument
{
    public:
        InvalidShiftException(const std::string& what_arg);
};
