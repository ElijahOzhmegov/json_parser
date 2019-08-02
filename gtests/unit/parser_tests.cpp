//
// Created by ilia on 13.06.19.
//

#include <gtest/gtest.h>
#include <string>
#include <fstream>

#define TEST_PARSER
#include "../../src/json.h"

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
    std::string array = "blah: [mistah: J, foo: bar],";
    std::string notStructure = "foo: bar, mistah: {big: B, lil: l},";

    ASSERT_TRUE (isFollowingStructure(structure));
    ASSERT_TRUE (isFollowingStructure(array));
    ASSERT_FALSE(isFollowingStructure(notStructure));

    ASSERT_EQ(isFollowingStructure(structure), 0x1);
    ASSERT_EQ(isFollowingStructure(array),     0x2);
}


TEST(convertStrToBool, HandlesConvertingStringToBool){
    ASSERT_TRUE  (strToBool("true"));
    ASSERT_FALSE (strToBool("True"));
    ASSERT_FALSE (strToBool("alskdjfls"));
    ASSERT_FALSE (strToBool("false"));

}

TEST(readingKey, HandlesDifferentOptionsOfKeys){
    std::size_t cursor = 0;

    EXPECT_EQ(readingKey("foo \"bar\", smth", cursor), "bar");
    EXPECT_NE(readingKey("foo bar, smth",     cursor), "bar");
}

TEST(readingVal, HandlesDifferentOptionsOfValue){
    std::size_t cursor = 0;

    EXPECT_EQ(readingVal("foo: bar , smth", cursor), " bar "); cursor=0;
    EXPECT_EQ(readingVal("foo: 1111bbbbcccc ,", cursor), " 1111bbbbcccc "); cursor=0;
    EXPECT_EQ(readingVal("foo:1111bbbbcccc,", cursor), "1111bbbbcccc");
}

TEST(choosingBorderType, HandlesDifferentBracketTypes){
    char lBorder;
    char rBorder;

    choosingBorderType(lBorder, rBorder, 0x1);

    EXPECT_EQ(lBorder, '{');
    EXPECT_EQ(rBorder, '}');

    choosingBorderType(lBorder, rBorder, 0x2);

    EXPECT_EQ(lBorder, '[');
    EXPECT_EQ(rBorder, ']');

}

TEST(findingBrackets, HandlesDifferentStructures){
    std::size_t cursor = 0;

    std::string structure = "blah: {mistah: J, foo: bar},";
    std::string array     = "blah: [mistah: J, foo: bar],";

    EXPECT_EQ(findingStucture(structure, cursor), "{mistah: J, foo: bar}"); cursor=0;
    EXPECT_EQ(findingArray   (array,     cursor), "[mistah: J, foo: bar]"); cursor=0;
}

TEST(readingField, HandlesDifferentStructures){
    std::string line_1 = "\"GainControl\":{\"GainAuto\":false,\"GainRaw\":42},\"Exposu";
    std::string line_2 = "   \"required\": [\"id\", \"name\", \"price\"],";

    std::size_t cursor = 0;
    std::pair <std::string,std::string> field;


    field = readingField(line_1, cursor); cursor = 0;
    EXPECT_EQ(field.first, "GainControl");
    EXPECT_EQ(field.second, "{\"GainAuto\":false,\"GainRaw\":42}");

    field = readingField(line_2, cursor); cursor = 0;
    EXPECT_EQ(field.first, "required");
    EXPECT_EQ(field.second, "[\"id\", \"name\", \"price\"]");
}

TEST(readingStructure, HandlesSimpleStructure){
    std::string line =  "{"
                        "        \"Mode\": 2,"
                        "        \"AutoTargetValue\": 128,"
                        "        \"AutoFunctionProfile\" : \"None\","
                        "        \"PixelFormat\": \"YUV422\","
                        "        \"Resolution\": \"1224x1024\","
                        "        \"AcquisitionFrameRateAbs\": 1.9,"
                        "        \"FlashOn\": true"
                        "}";

    std::map  <std::string, std::string> fields = readingStructure(line);
    std::map<std::string,std::string>::iterator it;


    it = fields.find("Mode");
    EXPECT_EQ(it->second, " 2");

    it = fields.find("AutoTargetValue");
    EXPECT_EQ(it->second, " 128");

    it = fields.find("PixelFormat");
    EXPECT_EQ(it->second, " \"YUV422\"");

    it = fields.find("Resolution");
    EXPECT_EQ(it->second, " \"1224x1024\"");

    it = fields.find("AcquisitionFrameRateAbs");
    EXPECT_EQ(it->second, " 1.9");

    it = fields.find("FlashOn");
    EXPECT_EQ(it->second, " true");
}

TEST(readingStructure, HandlesRegularStructure){
    std::string line =  "{"
                        "    \"Mode\": 2,"
                        "    \"GainControl\": {"
                        "        \"GainAuto\": false,"
                        "        \"GainRaw\": 42"
                        "    },"
                        "    \"Exposure\": {"
                        "        \"ExposureAuto\": true,"
                        "        \"ExposureTimeRaw\": 0"
                        "    },"
                        "    \"BalanceWhite\": {"
                        "        \"BalanceWhiteAuto\": false,"
                        "        \"BalanceRatioRaw\": 1.2"
                        "    },"
                        "    \"AutoTargetValue\": 128,"
                        "    \"AutoFunctionProfile\" : \"None\","
                        "    \"PixelFormat\": \"YUV422\","
                        "    \"Resolution\": \"1224x1024\","
                        "    \"AcquisitionFrameRateAbs\": 1.9,"
                        "    \"FlashOn\": true"
                        "}";

    std::map  <std::string, std::string> fields = readingStructure(line);
    std::map<std::string,std::string>::iterator it;


    it = fields.find("GainControl");
    EXPECT_EQ(it->second,   "{"
                            "        \"GainAuto\": false,"
                            "        \"GainRaw\": 42"
                            "    }");

    it = fields.find("Exposure");
    EXPECT_EQ(it->second,   "{"
                            "        \"ExposureAuto\": true,"
                            "        \"ExposureTimeRaw\": 0"
                            "    }");

    it = fields.find("BalanceWhite");
    EXPECT_EQ(it->second,   "{"
                            "        \"BalanceWhiteAuto\": false,"
                            "        \"BalanceRatioRaw\": 1.2"
                            "    }");
    it = fields.find("Mode");
    EXPECT_EQ(it->second, " 2");

    it = fields.find("AutoTargetValue");
    EXPECT_EQ(it->second, " 128");

    it = fields.find("PixelFormat");
    EXPECT_EQ(it->second, " \"YUV422\"");

    it = fields.find("Resolution");
    EXPECT_EQ(it->second, " \"1224x1024\"");

    it = fields.find("AcquisitionFrameRateAbs");
    EXPECT_EQ(it->second, " 1.9");

    it = fields.find("FlashOn");
    EXPECT_EQ(it->second, " true");
}

TEST(readingStructure, HandlesComplexStructure){
    std::string line =  "{"
                        "    \"$schema\": \"http://json-schema.org/schema#\","
                        "            \"title\": \"Product\","
                        "            \"type\": \"object\","
                        "            \"required\": [\"id\", \"name\", \"price\"],"
                        "    \"properties\": {"
                        "        \"id\": {"
                        "            \"type\": \"number\","
                        "                    \"description\": \"Product identifier\""
                        "        },"
                        "        \"name\": {"
                        "            \"type\": \"string\","
                        "                    \"description\": \"Name of the product\""
                        "        },"
                        "        \"price\": {"
                        "            \"type\": \"number\","
                        "                    \"minimum\": 0"
                        "        },"
                        "        \"tags\": {"
                        "            \"type\": \"array\","
                        "                    \"items\": {"
                        "                \"type\": \"string\""
                        "            }"
                        "        },"
                        "        \"stock\": {"
                        "            \"type\": \"object\","
                        "                    \"properties\": {"
                        "                \"warehouse\": {"
                        "                    \"type\": \"number\""
                        "                },"
                        "                \"retail\": {"
                        "                    \"type\": \"number\""
                        "                }"
                        "            }"
                        "        }"
                        "    }"
                        "}";

    std::map  <std::string, std::string> fields = readingStructure(line);
    std::map<std::string,std::string>::iterator it;


    it = fields.find("$schema");
    EXPECT_EQ(it->second, " \"http://json-schema.org/schema#\"");

    it = fields.find("required");
    EXPECT_EQ(it->second, "[\"id\", \"name\", \"price\"]");

    it = fields.find("properties");
    EXPECT_EQ(it->second,   "{"
                            "        \"id\": {"
                            "            \"type\": \"number\","
                            "                    \"description\": \"Product identifier\""
                            "        },"
                            "        \"name\": {"
                            "            \"type\": \"string\","
                            "                    \"description\": \"Name of the product\""
                            "        },"
                            "        \"price\": {"
                            "            \"type\": \"number\","
                            "                    \"minimum\": 0"
                            "        },"
                            "        \"tags\": {"
                            "            \"type\": \"array\","
                            "                    \"items\": {"
                            "                \"type\": \"string\""
                            "            }"
                            "        },"
                            "        \"stock\": {"
                            "            \"type\": \"object\","
                            "                    \"properties\": {"
                            "                \"warehouse\": {"
                            "                    \"type\": \"number\""
                            "                },"
                            "                \"retail\": {"
                            "                    \"type\": \"number\""
                            "                }"
                            "            }"
                            "        }"
                            "    }");
}
