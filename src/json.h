//
// Created by Elijah Ozhmegov on 2019-05-24.
//

#ifndef JSON_PARSER_JSON_H
#define JSON_PARSER_JSON_H

#include <string>

struct Settings {
    Settings();
    void extract(const char * from);

private:
    struct GainControl{
        bool GainAuto;
        int GainRaw;
    } gain;

    struct Exposure{
        bool ExposureAuto;
        int ExposureTimeRaw;
    } exposure;
    struct BalanceWhite{
        bool BalanceWhiteAuto;
        float BalanceWhiteRaw;
    } balance;

    int AutoTargetValue;
    std::string PixelFormat;

    struct Resolution{
        int Width;
        int Height;
    } resolution;

    float AcqusitionFrameRateAbs;
    bool FlashOn;
};


#endif //JSON_PARSER_JSON_H

