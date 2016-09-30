#pragma once
#include <string>

class CaesarCipher {
    public:
        std::string Generate(const std::string& message, const int shift);
    private:
        static const int _ascii_range = 128;
        static const int _first_ascii_char = 0;
};
