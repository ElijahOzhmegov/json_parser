//
// Created by Elijah Ozhmegov on 2019-05-24.
//

#ifndef JSON_PARSER_JSON_H
#define JSON_PARSER_JSON_H

#include <string>
#include <map>

struct Settings {
private:
    void fillingSettings    (std::map<std::string, std::string> & fields);

    void fillingGainControl (std::string line);
    void fillingExposure    (std::string line);
    void fillingBalanceWhite(std::string line);

public:
    explicit Settings();
    void extract(const char * from);

private:
    struct GainControl{
        bool GainAuto;
        int  GainRaw;
    } GainControl;

    struct Exposure{
        bool ExposureAuto;
        int  ExposureTimeRaw;
    } Exposure;

    struct BalanceWhite{
        bool  BalanceWhiteAuto;
        float BalanceWhiteRaw;
    } BalanceWhite;

    int AutoTargetValue;
    std::string PixelFormat;

    struct Resolution{
        int Width;
        int Height;
    } Resolution;

    double AcquisitionFrameRateAbs;
    bool   FlashOn;
};


#ifdef TEST_PARSER

bool isValidFile(std::ifstream & file);
std::string removingSpaces(std::ifstream & file);
std::string extractFromQmarks(std::string & line);
bool isBracketsValid(std::size_t l, std::size_t r);
bool isFollowingStructure(const std::string & line, std::size_t from=0);
bool convertStrToBool(std::string line);
std::string readingKey(const std::string & line, std::size_t & cursor);
std::string readingVal(const std::string & line, std::size_t & cursor);

#endif //TEST_PARSER

#endif //JSON_PARSER_JSON_H

