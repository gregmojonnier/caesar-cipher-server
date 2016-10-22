#pragma once
#include <vector>
#include <string>
class IDataInterpreter {
    public:
        virtual ~IDataInterpreter() = default;
        virtual std::vector<std::string> SplitDataIntoCompleteWords(char* buffer, std::size_t num_bytes) = 0;
        virtual std::string InterpretWordsAndDetermineResponse(const std::vector<std::string>& complete_words) = 0;
};
