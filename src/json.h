//
// Created by Elijah Ozhmegov on 2019-05-24.
//

#ifndef JSON_PARSER_JSON_H
#define JSON_PARSER_JSON_H

#include <string>
#include <map>

struct Settings {
private:
    void fillingSettings(std::map<std::string, std::string> & fields);
    void fillingStruct( const std::string & name,
                        const std::string & nameAuto,
                        const std::string & nameRaw,
                        std::map<std::string, std::string> & from);

public:
    explicit Settings();
    void extract(const char * from);


    int get_mode()             { return Mode;};
    int get_auto_target_value(){ return AutoTargetValue;};

    std::string get_auto_func_profile(){ return AutoFunctionProfile;};
    std::string get_pixel_format()     { return PixelFormat;};
    std::string get_resolution()       { return Resolution;};

    double get_acquisition_frame_rate()  { return  AcquisitionFrameRateAbs;};
    bool   get_flash_on() { return FlashOn;};

    bool get_GA() { return GainControl.GainAuto; };
    bool get_EA() { return Exposure.ExposureAuto; };
    bool get_BA() { return BalanceWhite.BalanceWhiteAuto; };

    int    get_GR() { return GainControl.GainRaw; };
    int    get_ER() { return Exposure.ExposureTimeRaw; };
    double get_BR() { return BalanceWhite.BalanceWhiteRaw; };

private:
    int Mode;

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
        double BalanceWhiteRaw;
    } BalanceWhite;

    int AutoTargetValue;

    std::string AutoFunctionProfile;
    std::string PixelFormat;
    std::string Resolution;

    double AcquisitionFrameRateAbs;
    bool   FlashOn;
};


#ifdef TEST_PARSER

bool isValidFile(std::ifstream & file);
std::string removingSpaces(std::ifstream & file);
std::string extractFromQmarks(std::string & line);
bool isBracketsValid(std::size_t l, std::size_t r);
bool isFollowingStructure(const std::string & line, std::size_t from=0);
bool strToBool(std::string line);
std::string readingKey(const std::string & line, std::size_t & cursor);
std::string readingVal(const std::string & line, std::size_t & cursor);
void choosingBorderType(char & lBorder, char & rBorder, unsigned char bracketType);
std::string findingStucture(const std::string & line, std::size_t & cursor);
std::string findingArray(const std::string & line, std::size_t & cursor);
std::pair <std::string,std::string> readingField(const std::string & line, std::size_t & cursor);
std::map  <std::string, std::string>
        readingStructure(const std::string & line, std::size_t cursor=0);
std::string
        extractingValue(std::map<std::string, std::string> & from, const std::string key);

#endif //TEST_PARSER

#endif //JSON_PARSER_JSON_H

