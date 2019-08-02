//
// Created by Elijah Ozhmegov on 2019-05-24.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <utility>      // std::pair, std::make_pair
#include <stdlib.h>     /* atof */
#include <cstdlib>

#include "json.h"

namespace {
    static const unsigned char isStructure = 0x1;
    static const unsigned char isArray     = 0x2;
};

Settings::Settings(){
    GainControl.GainAuto = false;
    GainControl.GainRaw  = 0;

    Exposure.ExposureAuto     = false;
    Exposure.ExposureTimeRaw  = 0;

    BalanceWhite.BalanceWhiteAuto = false;
    BalanceWhite.BalanceWhiteRaw  = 0;

    AutoTargetValue = 0;
    PixelFormat     = "YUV422";


    AcquisitionFrameRateAbs = 1;
    FlashOn = true;
}

bool isValidFile(std::ifstream & file){
    if (!file)
        return false;
    else
        return true;
}

// removing line separator as well
std::string removingSpaces(std::ifstream & file){
    using namespace std;
    string data;

    char c = 0;
    file.get(c);

    while (file.good()) {
        if ((c != '\n') && (c != ' ')) data.append(1, c);
        file.get(c);
    }

    return data;

}

std::string extractFromQmarks(std::string & line){
    std::size_t lQmark = line.find('"');
    std::size_t rQmark = line.find('"', ++lQmark);

    return line.substr(lQmark, rQmark - lQmark);
}

bool isBracketsValid(std::size_t l, std::size_t r){
    return  !((l == std::string::npos) ||
              (r == std::string::npos) ||
              (l > r));
}

unsigned char
isFollowingStructure(const std::string & line, std::size_t from=0){
    std::size_t comma   = line.find(',', from);
    std::size_t brace   = line.find('{', from);
    std::size_t bracket = line.find('[', from);

    return (unsigned char) ((brace < comma)*isStructure + (bracket < comma)*isArray);
}

bool strToBool(std::string line){
    return  (line == "true");
}

std::string readingKey(const std::string & line, std::size_t & cursor){
    std::size_t lQmark = line.find('"', cursor);
    std::size_t rQmark = line.find('"', ++lQmark);

    cursor = rQmark;

    return line.substr(lQmark, rQmark - lQmark);
}

std::string readingVal(const std::string & line, std::size_t & cursor){
    std::size_t lVal = line.find(':', cursor);
    std::size_t rVal = line.find(',', ++lVal);

    cursor = rVal;

    return line.substr(lVal, rVal - lVal);
}

void choosingBorderType(char & lBorder, char & rBorder, unsigned char bracketType){
    if (bracketType == isStructure){
        lBorder = '{';
        rBorder = '}';
    } else if(bracketType == isArray){
        lBorder = '[';
        rBorder = ']';
    }else{
        std::cout << "Unknown Structure type" << std::endl;
    }
}

std::string findingBrackets(const std::string & line, std::size_t & cursor, unsigned char bracketType){
    char lBorder;
    char rBorder;

    choosingBorderType(lBorder, rBorder, bracketType);

    std::size_t lBracket = line.find(lBorder, cursor);
    std::size_t rBracket = line.find(rBorder, cursor);

    std::size_t rigidL = lBracket;
    std::size_t rigidR = rBracket;

    int count = 1;

    do{
        std::size_t tmpL = line.find(lBorder, ++lBracket);

        while (tmpL < rBracket){
            count++;
            lBracket = tmpL;
            tmpL = line.find(lBorder, ++tmpL);
        }

        if (count > 0){
            rigidR = rBracket;
            rBracket = line.find(rBorder, ++rBracket);
            count--;
        }
    }while (count != 0);

    cursor = ++rigidR;

    return line.substr(rigidL, rigidR - rigidL);
}

std::string findingStucture(const std::string & line, std::size_t & cursor){
    return findingBrackets(line, cursor, isStructure);
}

std::string findingArray(const std::string & line, std::size_t & cursor){
    return findingBrackets(line, cursor, isArray);
}

std::pair <std::string,std::string>
readingField(const std::string & line, std::size_t & cursor){
    std::string key = readingKey(line, cursor);
    std::string val;

    unsigned char structureType = isFollowingStructure(line, ++cursor);

    if (structureType){
        val = findingBrackets(line, cursor, structureType);
    }else{
        val = readingVal(line, cursor);
    }

    std::pair <std::string,std::string> field (key, val);

    return field;
}

std::map  <std::string, std::string>
readingStructure(const std::string & line, std::size_t cursor=0){
    std::size_t lBracket = line.find ('{');
    std::size_t rBracket = line.rfind('}');

    std::pair <std::string, std::string> field;
    std::map  <std::string, std::string> fields;

    if (!isBracketsValid(lBracket, rBracket))
        return fields;
    lBracket++;
    rBracket--;


    while(cursor < rBracket){
        field = readingField(line.substr(lBracket, rBracket), cursor);
        fields[field.first] = field.second;
    }

    return fields;
}

std::string extractingValue(std::map<std::string, std::string> & from, std::string key){
    std::map<std::string,std::string>::iterator it;
    it = from.find(key);

    return it->second;
}

void Settings::fillingStruct(   const std::string & name,
                                const std::string & nameAuto,
                                const std::string & nameRaw,
                                std::map<std::string, std::string> & from) {

    std::string value = extractingValue(from, name);
    std::map  <std::string, std::string> fields = readingStructure(value);
    std::map<std::string,std::string>::iterator it;

    it = fields.find(nameAuto);
    if (it == fields.end()) return;

    if (nameAuto == "GainAuto")
        GainControl.GainAuto          = strToBool(it->second);
    if (nameAuto == "ExposureAuto")
        Exposure.ExposureAuto         = strToBool(it->second);
    if (nameAuto == "BalanceWhiteAuto")
        BalanceWhite.BalanceWhiteAuto = strToBool(it->second);

    it = fields.find(nameRaw);
    if (it == fields.end()) return;

    if (nameAuto == "GainAuto")
        GainControl.GainRaw          = atoi(it->second.c_str());
    if (nameAuto == "ExposureAuto")
        Exposure.ExposureTimeRaw     = atoi(it->second.c_str());
    if (nameAuto == "BalanceWhiteAuto")
        BalanceWhite.BalanceWhiteRaw = strtod(it->second.c_str(), NULL);
}

void Settings::fillingSettings(std::map<std::string, std::string> & fields){
    Mode                    = atoi(   extractingValue(fields, "Mode"           ).c_str() );
    AutoTargetValue         = atoi(   extractingValue(fields, "AutoTargetValue").c_str() );
    AcquisitionFrameRateAbs = strtod( extractingValue(fields, "AcquisitionFrameRateAbs").c_str(), NULL );

    AutoFunctionProfile = extractingValue(fields, "AutoFunctionProfile");
    PixelFormat         = extractingValue(fields, "PixelFormat");
    Resolution          = extractingValue(fields, "Resolution");

    AutoFunctionProfile = extractFromQmarks(AutoFunctionProfile);
    PixelFormat         = extractFromQmarks(PixelFormat);
    Resolution          = extractFromQmarks(Resolution);

    FlashOn = strToBool(extractingValue(fields, "FlashOn"));


    fillingStruct("GainControl",  "GainAuto",         "GainRaw",         fields);
    fillingStruct("Exposure",     "ExposureAuto",     "ExposureTimeRaw", fields);
    fillingStruct("BalanceWhite", "BalanceWhiteAuto", "BalanceRatioRaw", fields);
}

void Settings::extract(const char * from){
    std::ifstream file(from, std::ios_base::in);

    if (!isValidFile(file)) return;

    std::string dataLine = removingSpaces(file);
    std::map  <std::string, std::string> fields = readingStructure(dataLine);
    fillingSettings(fields);
    file.close();
}

