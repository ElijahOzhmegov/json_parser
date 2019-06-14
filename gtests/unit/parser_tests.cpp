//
// Created by ilia on 13.06.19.
//

#include <gtest/gtest.h>
#include <string>
#include <fstream>

#define TEST_PARSER

#include "../src/json.h"


using namespace std;

TEST(isValidFile, HandlesRealFile){
    const char * filename("../test.json");
    std::ifstream file(filename, std::ios_base::in);

    ASSERT_TRUE(isValidFile(file));
}


