#include "caesar_cipher_data_interpreter.h"
#include <cstring>
#include <sstream>

CaesarCipherDataInterpreter::CaesarCipherDataInterpreter() : _is_shift(true)
{
}

std::vector<std::string> CaesarCipherDataInterpreter::SplitDataIntoCompleteWords(char* buffer, std::size_t num_bytes)
{
    std::vector<std::string> words;
    if (buffer == NULL || num_bytes == 0)
        return words;

    std::string data;
    // Starting data to split is any left over partial word(contiguous characters without a space)
    // plus the newly received data
    data.reserve(num_bytes + _partial_word.size());
    data = _partial_word;
    data.append(buffer, num_bytes);

    // New word indices
    std::size_t start_idx = 0;
    std::size_t space_idx = data.find(" ", start_idx);

    // Completing the first word means we used up any partial word
    bool partial_completed = false;
    bool found_space = space_idx != std::string::npos;
    if (found_space)
        partial_completed = true;

    while (found_space) {
        auto word = data.substr(start_idx, space_idx - start_idx);
        if (!word.empty())
            words.push_back(std::move(word));
        start_idx = space_idx + 1;
        space_idx = data.find(" ", start_idx);
        found_space = space_idx != std::string::npos;
    }

    // Clean up existing partial word if used and store any that was newly received
    if (partial_completed)
        _partial_word.clear();

    if (start_idx != data.size())
        _partial_word = data.substr(start_idx);

    return words;
}

std::string CaesarCipherDataInterpreter::InterpretWordsAndDetermineResponse(const std::vector<std::string>& complete_words)
{
    std::string response;

    std::string message;
    for (auto& word: complete_words) {
        if (_is_shift) {
            std::stringstream s;
            s << word;
            s >> _shift;
            if (!s) {
                throw InvalidShiftException("Shift given was not a valid integer: " + word);
            }
            _is_shift = false;
        } else {
            response += _caesar_cipher.Generate(word, _shift) + " ";
            _is_shift = true;
        }
    }

    return response;
}

InvalidShiftException::InvalidShiftException(const std::string& what_arg) : std::invalid_argument(what_arg)
{
}
