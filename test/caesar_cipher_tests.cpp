#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/caesar_cipher.h"

TEST(CaesarCipher, Generate_EmptyMessageWithShiftOfZero_ReturnsEmptyString) {
    CaesarCipher c;

    std::string cipher = c.Generate("", 0);

    EXPECT_THAT(cipher, ::testing::IsEmpty());
}

TEST(CaesarCipher, Generate_EmptyMessageWithNonZeroShift_ReturnsEmptyString) {
    CaesarCipher c;

    std::string cipher = c.Generate("", 32);

    EXPECT_THAT(cipher, ::testing::IsEmpty());
}

TEST(CaesarCipher, Generate_MessageWithShiftOfZero_ReturnsUnchangedMessage) {
    CaesarCipher c;

    std::string cipher = c.Generate("ABC_message", 0);

    EXPECT_THAT(cipher, ::testing::StrEq("ABC_message"));
}

TEST(CaesarCipher, Generate_MessageOfOneCharWithShiftOfOne_ReturnsCharShiftedOne) {
    CaesarCipher c;

    std::string cipher = c.Generate("A", 1);

    EXPECT_THAT(cipher, ::testing::StrEq("B"));
}

TEST(CaesarCipher, Generate_MessageMultipleCharsWithShiftOfOne_ReturnsAllCharsShiftedOne) {
    CaesarCipher c;

    std::string cipher = c.Generate("ABC", 1);

    EXPECT_THAT(cipher, ::testing::StrEq("BCD"));
}

TEST(CaesarCipher, Generate_MessageMultipleCharsWithShiftOfThree_ReturnsAllCharsShiftedThree) {
    CaesarCipher c;

    std::string cipher = c.Generate("ABC", 3);

    EXPECT_THAT(cipher, ::testing::StrEq("DEF"));
}

TEST(CaesarCipher, Generate_MessageOfLastAsciiCharWithShiftOfOne_LoopsAroundReturningFirstAsciiChar) {
    CaesarCipher c;
    std::string last_ascii_char_message(1, '\x7f');
    std::string first_ascii_char_message(1, '\x0');

    std::string cipher = c.Generate(last_ascii_char_message, 1);

    EXPECT_THAT(cipher, ::testing::StrEq(first_ascii_char_message));
}

TEST(CaesarCipher, Generate_MessageWithShiftOfAsciiRange_LoopsAroundReturningItself) {
    CaesarCipher c;

    std::string cipher = c.Generate("abcd", 128);

    EXPECT_THAT(cipher, ::testing::StrEq("abcd"));
}

TEST(CaesarCipher, Generate_MessageOfOneCharWithShiftOfNegativeOne_ReturnsCharShiftedNegativeOne) {
    CaesarCipher c;

    std::string cipher = c.Generate("B", -1);

    EXPECT_THAT(cipher, ::testing::StrEq("A"));
}

TEST(CaesarCipher, Generate_MessageMultipleCharsWithShiftOfNegativeOne_ReturnsAllCharsShiftedNegativeOne) {
    CaesarCipher c;

    std::string cipher = c.Generate("BCD", -1);

    EXPECT_THAT(cipher, ::testing::StrEq("ABC"));
}

TEST(CaesarCipher, Generate_MessageMultipleCharsWithShiftOfNegativeThree_ReturnsAllCharsShiftedNegativeThree) {
    CaesarCipher c;

    std::string cipher = c.Generate("DEF", -3);

    EXPECT_THAT(cipher, ::testing::StrEq("ABC"));
}

TEST(CaesarCipher, Generate_MessageOfFirstAsciiCharWithShiftOfNegativeOne_LoopsAroundReturningLastAsciiChar) {
    CaesarCipher c;
    std::string last_ascii_char_message(1, '\x7f');
    std::string first_ascii_char_message(1, '\x0');

    std::string cipher = c.Generate(first_ascii_char_message, -1);

    EXPECT_THAT(cipher, ::testing::StrEq(last_ascii_char_message));
}

TEST(CaesarCipher, Generate_MessageWithShiftOfNegativeAsciiRange_LoopsAroundReturningItself) {
    CaesarCipher c;

    std::string cipher = c.Generate("abcd", -128);

    EXPECT_THAT(cipher, ::testing::StrEq("abcd"));
}

TEST(CaesarCipher, Generate_ReallyLargePositiveShift_ReturnsTheCorrectCipher) {
    CaesarCipher c;
    int really_large_shift = (128 * 5) + 1; // ends up just shifting 1 to the right

    std::string cipher = c.Generate("A", really_large_shift);

    EXPECT_THAT(cipher, ::testing::StrEq("B"));
}

TEST(CaesarCipher, Generate_ReallyLargeNegativeShift_ReturnsTheCorrectCipher) {
    CaesarCipher c;
    int really_large_shift = (-128 * 5) - 1; // ends up just shifting 1 to the left

    std::string cipher = c.Generate("B", really_large_shift);

    EXPECT_THAT(cipher, ::testing::StrEq("A"));
}

TEST(CaesarCipher, Generate_MessageOfCapitalizedAlphabetWithShiftOfDistanceToLowercased_ReturnsTheLowercasedAlphabet) {
    CaesarCipher c;
    int capital_to_lowercase_distance = 32;

    std::string cipher = c.Generate("ABCDEFGHIJKLMNOPQRSTUVWXYZ", capital_to_lowercase_distance);

    EXPECT_THAT(cipher, ::testing::StrEq("abcdefghijklmnopqrstuvwxyz"));
}
