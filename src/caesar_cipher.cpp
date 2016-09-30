#include "caesar_cipher.h"

std::string CaesarCipher::Generate(const std::string& message, const int shift) {
    std::string ciphered_message;
    for (const auto& character : message) {
        int shifted_char = int(character) + shift;

        // normalize shifted char in order to wrap shifts outside ascii range around
        int normalized_char = (shifted_char) % _ascii_range;
        if (shifted_char < _first_ascii_char) {
            normalized_char += _ascii_range;
        }

        ciphered_message += char(normalized_char);
    }

    return ciphered_message;
}
