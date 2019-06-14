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
    ifstream file;

    file.open("../test_files/test.json", ios_base::in);
    ASSERT_TRUE(isValidFile(file));

    file.open((const char *) "foo", ios_base::in);
    ASSERT_FALSE(isValidFile(file));
}


TEST(removingSpaces, HandlesRemovingSpaces){
    ifstream file;

    file.open("../test_files/space_test.txt", ios_base::in);
    EXPECT_EQ(removingSpaces(file), "blablabla123anotheline");

}

TEST(removingQmarks, HandlesRemovingQuotationMark){

    string first = " 123 ";
    string _first = "123 \" 123 \" ";

    ASSERT_EQ(removingQmarks(_first), first );
//    ASSERT_EQ(removingQmarks("123  123"), "123  123");
}