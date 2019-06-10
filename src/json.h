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

    void fillingGainControl (std::string line);
    void fillingExposure    (std::string line);
    void fillingBalanceWhite(std::string line);

public:
    explicit Settings();
    void extract(const char * from);

private:
    struct GainControl{
        bool GainAuto;
        int GainRaw;
    } GainControl;

    struct Exposure{
        bool ExposureAuto;
        int ExposureTimeRaw;
    } Exposure;

    struct BalanceWhite{
        bool BalanceWhiteAuto;
        float BalanceWhiteRaw;
    } BalanceWhite;

    int AutoTargetValue;
    std::string PixelFormat;

    struct Resolution{
        int Width;
        int Height;
    } Resolution;

    double AcquisitionFrameRateAbs;
    bool FlashOn;
};


#endif //JSON_PARSER_JSON_H

