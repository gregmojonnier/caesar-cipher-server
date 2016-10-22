#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/caesar_cipher_data_interpreter.h"

using ::testing::IsEmpty;
using ::testing::Contains;
using ::testing::StrEq;
using ::testing::SizeIs;
using ::testing::AllOf;

class CaesarCipherDataInterpreterTests : public ::testing::Test {
    protected:
        CaesarCipherDataInterpreter _test_subject;
};

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_NullBuffer_ReturnsEmptyWordsVector)
{
    char* null_buffer = NULL;
    size_t num_bytes = 0;

    auto words = _test_subject.SplitDataIntoCompleteWords(null_buffer, num_bytes);

    EXPECT_THAT(words, IsEmpty());
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_ZeroBytesBuffer_ReturnsEmptyWordsVector)
{
    char buffer_with_some_chars[] = "abc ";
    size_t num_bytes = 0;

    auto words = _test_subject.SplitDataIntoCompleteWords(buffer_with_some_chars, num_bytes);

    EXPECT_THAT(words, IsEmpty());
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_EmptyStringBuffer_ReturnsEmptyWordsVector)
{
    char empty_string_buffer[] = "";
    size_t num_bytes = 1;

    auto words = _test_subject.SplitDataIntoCompleteWords(empty_string_buffer, num_bytes);

    EXPECT_THAT(words, IsEmpty());
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_BufferWithOneSpace_ReturnsEmptyVector)
{
    char buffer_with_space[] = " ";
    size_t num_bytes = 1;

    auto words = _test_subject.SplitDataIntoCompleteWords(buffer_with_space, num_bytes);

    EXPECT_THAT(words, IsEmpty());
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_BufferWithCharsButNoSpace_ReturnsEmptyWordsVector)
{
    char non_empty_buffer_without_space[] = "word_without_space";
    size_t num_bytes = strlen(non_empty_buffer_without_space);

    auto words = _test_subject.SplitDataIntoCompleteWords(non_empty_buffer_without_space, num_bytes);

    EXPECT_THAT(words, IsEmpty());
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_WordsWithoutSpacesOverMultipleCalls_PersistUntilASpaceIsGiven)
{
    char word_without_space[] = "ab";
    char word_completer[] = "c ";
    char word_with_space[] = "def ";

    auto words1 = _test_subject.SplitDataIntoCompleteWords(word_without_space, strlen(word_without_space));
    auto words2 = _test_subject.SplitDataIntoCompleteWords(word_completer, strlen(word_completer));
    auto words3 = _test_subject.SplitDataIntoCompleteWords(word_with_space, strlen(word_with_space));

    EXPECT_THAT(words1, IsEmpty());
    EXPECT_THAT(words2, AllOf(
                            SizeIs(1),
                            Contains(StrEq("abc"))
                            ));
    EXPECT_THAT(words3, AllOf(
                            SizeIs(1),
                            Contains(StrEq("def"))
                            ));
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_BufferWithOneWordWithTrailingSpace_ReturnsVectorWithTheWord)
{
    char one_word_with_trailing_space[] = "one_word ";
    size_t num_bytes = strlen(one_word_with_trailing_space);

    auto words = _test_subject.SplitDataIntoCompleteWords(one_word_with_trailing_space, num_bytes);

    EXPECT_THAT(words, Contains(StrEq("one_word")));
    EXPECT_THAT(words, SizeIs(1));
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_BufferWithTwoSpaceSeparatedWords_ReturnsVectorWithTheFirstWord)
{
    char two_space_separated_words[] = "first_word second_word";
    size_t num_bytes = strlen(two_space_separated_words);

    auto words = _test_subject.SplitDataIntoCompleteWords(two_space_separated_words, num_bytes);

    EXPECT_THAT(words, Contains(StrEq("first_word")));
    EXPECT_THAT(words, SizeIs(1));
}

TEST_F(CaesarCipherDataInterpreterTests, SplitDataIntoCompleteWords_BufferWithTwoSpaceSeparatedWordsAndTrailingSpace_ReturnsVectorWithBothWords)
{
    char two_complete_words[] = "first_word second_word ";
    size_t num_bytes = strlen(two_complete_words);

    auto words = _test_subject.SplitDataIntoCompleteWords(two_complete_words, num_bytes);

    EXPECT_THAT(words, AllOf(Contains(StrEq("first_word")),
                             Contains(StrEq("second_word"))));
    EXPECT_THAT(words, SizeIs(2));
}

TEST_F(CaesarCipherDataInterpreterTests, InterpretWordsAndDetermineResponse_NoCompleteWords_ReturnsEmptyResponse)
{
    std::vector<std::string> complete_words;

    auto response = _test_subject.InterpretWordsAndDetermineResponse(complete_words);

    EXPECT_THAT(response, IsEmpty());
}

TEST_F(CaesarCipherDataInterpreterTests, InterpretWordsAndDetermineResponse_FirstWordIsShiftSecondIsMessage_ResponseIsCorrectlyShiftedMessageWithTrailingSpace)
{
    std::vector<std::string> complete_words {"1", "abc"};

    auto response = _test_subject.InterpretWordsAndDetermineResponse(complete_words);

    EXPECT_THAT(response, StrEq("bcd "));
}

TEST_F(CaesarCipherDataInterpreterTests, InterpretWordsAndDetermineResponse_ShiftIsNotAValidNumber_ThrowsInvalidShiftException)
{
    std::vector<std::string> complete_words {"not_a_number", "abc"};

    EXPECT_THROW(_test_subject.InterpretWordsAndDetermineResponse(complete_words), InvalidShiftException);
}

TEST_F(CaesarCipherDataInterpreterTests, InterpretWordsAndDetermineResponse_MultipleCompleteMessages_ResponseIsAllCompleteCorrectlyShiftedMessages)
{
    std::vector<std::string> complete_words {"3", "abc",
                                             "2", "abc",
                                             "1", "abc"};

    auto response = _test_subject.InterpretWordsAndDetermineResponse(complete_words);

    EXPECT_THAT(response, StrEq("def cde bcd "));
}

TEST_F(CaesarCipherDataInterpreterTests, InterpretWordsAndDetermineResponse_ExpectedCurrentWordState_IsMaintainedBetweenConsecutiveCalls)
{
    std::vector<std::string> just_shift_word {"-1"};
    std::vector<std::string> just_message_word {"bcd"};
    std::vector<std::string> complete_message {"1", "abc"};

    auto response1 = _test_subject.InterpretWordsAndDetermineResponse(just_shift_word);
    auto response2 = _test_subject.InterpretWordsAndDetermineResponse(just_message_word);
    auto response3 = _test_subject.InterpretWordsAndDetermineResponse(complete_message);

    EXPECT_THAT(response1, IsEmpty());
    EXPECT_THAT(response2, StrEq("abc "));
    EXPECT_THAT(response3, StrEq("bcd "));
}
