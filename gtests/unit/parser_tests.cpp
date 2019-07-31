//
// Created by ilia on 13.06.19.
//

#include <gtest/gtest.h>
#include <string>
#include <fstream>

#define TEST_PARSER
#include "../src/json.h"

using namespace std;

TEST(isValidFile, HandlesReadingFile){
    const char * filename("../test_files/test.json");
    ifstream file;

    file.open(filename, ios_base::in);
    ASSERT_TRUE(isValidFile(file));

    file.open((const char *) "foo", ios_base::in);
    ASSERT_FALSE(isValidFile(file));
}


TEST(removingSpaces, HandlesRemovingSpaces){
    const char * filename("../test_files/space_test.txt");
    ifstream file;

    file.open(filename, ios_base::in);
    EXPECT_EQ(removingSpaces(file), "blablabla123anotheline");
    EXPECT_NE(removingSpaces(file), "blablabla 123anotheline");

}

TEST(extractFromQmarks, HandlesRemovingQuotationMark){
    string first_ = "123 \" 123 \" ";
    string first  = " 123 ";

    string second_ = "111  \"sense\" 222";
    string second  = "sense";

    ASSERT_EQ(extractFromQmarks(first_), first );
    ASSERT_EQ(extractFromQmarks(second_), second );
}

TEST(isBracketsValid, HandlesDifferentBrackets){
    ASSERT_TRUE (isBracketsValid(10, 11));
    ASSERT_TRUE (isBracketsValid(11, 11));
    ASSERT_FALSE(isBracketsValid(12, 11));
}

TEST(isFollowingStructure, HandlesDifferentStructures){
    std::string structure = "blah: {mistah: J, foo: bar},";
    std::string diffrentStructure = "blah: [mistah: J, foo: bar],";
    std::string notStructure = "foo: bar, mistah: {big: B, lil: l},";

    ASSERT_TRUE (isFollowingStructure(structure));
    ASSERT_TRUE (isFollowingStructure(diffrentStructure));
    ASSERT_FALSE(isFollowingStructure(notStructure));
}


TEST(convertStrToBool, HandlesConvertingStringToBool){
    ASSERT_TRUE  (convertStrToBool("true"));
    ASSERT_FALSE (convertStrToBool("True"));
    ASSERT_FALSE (convertStrToBool("alskdjfls"));
    ASSERT_FALSE (convertStrToBool("false"));

}

TEST(readingKey, HandlesDifferentOptionsOfKeys){
    std::size_t cursor = 0;

    EXPECT_EQ(readingKey("foo \"bar\", smth", cursor), "bar");
    EXPECT_NE(readingKey("foo bar, smth",     cursor), "bar");
}

TEST(readingVal, HandlesDifferentOptionsOfValue){
    std::size_t cursor = 0;

    EXPECT_EQ(readingVal("foo: bar , smth", cursor), " bar "); cursor=0;
    EXPECT_EQ(readingVal("foo: 1111bbbbcccc ,", cursor), "1111bbbbcccc");
}
