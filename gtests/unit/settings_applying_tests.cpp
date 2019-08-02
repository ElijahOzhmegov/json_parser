//
// Created by ilia on 02.08.19.
//

#include <gtest/gtest.h>
#include <string>
#include <fstream>

#define TEST_PARSER
#include "../../src/json.h"

using namespace std;

namespace {
    static std::string line = "{"
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
}

TEST(extractingValue, HandlesSimpleExtracting){
    std::map  <std::string, std::string> fields = readingStructure(line);

    EXPECT_EQ(extractingValue(fields, "Mode"), " 2");
    EXPECT_EQ(extractingValue(fields, "Resolution"), " \"1224x1024\"");
}

TEST(fillingSettings, HandlesSimpleFilling){
    const char * filename("../../cam_config.json");

    Settings S;
    S.extract(filename);

    EXPECT_EQ(S.get_mode(), 2);
    EXPECT_EQ(S.get_auto_target_value(), 128);

    EXPECT_EQ(S.get_auto_func_profile(), "None");
    EXPECT_EQ(S.get_pixel_format(),      "YUV422");
    EXPECT_EQ(S.get_resolution(),        "1224x1024");

    EXPECT_EQ(S.get_acquisition_frame_rate(), 1.9);
    EXPECT_EQ(S.get_flash_on(), 1);
}

TEST(fillingSettings, HandlesComplexFilling){
    const char * filename("../../cam_config.json");

    Settings S;
    S.extract(filename);

    EXPECT_EQ(S.get_GA(), 0);
    EXPECT_EQ(S.get_EA(), 1);
    EXPECT_EQ(S.get_BA(), 0);

    EXPECT_EQ(S.get_GR(), 42);
    EXPECT_EQ(S.get_ER(), 0);
    EXPECT_EQ(S.get_BR(), 1.2);
}
