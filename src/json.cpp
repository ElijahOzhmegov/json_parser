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

Settings::Settings(){
    GainControl.GainAuto = false;
    GainControl.GainRaw  = 0;

    Exposure.ExposureAuto     = false;
    Exposure.ExposureTimeRaw  = 0;

    BalanceWhite.BalanceWhiteAuto = false;
    BalanceWhite.BalanceWhiteRaw  = 0;

    AutoTargetValue = 0;
    PixelFormat     = "YUV422";

    Resolution.Width  = 1224;
    Resolution.Height = 1024;

    AcquisitionFrameRateAbs = 25;
    FlashOn = true;
}

bool isValidFile(std::ifstream & file){
    if (!file) {
        std::cout << "Unfortunately, did not manage to open file\n\n";
        return false;
    }else{
        std::cout << "File was opened successfully!\n\n";
        return true;
    }
}

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

std::string removingQmarks(std::string & line){
    std::size_t lQmark = line.find('"');
    std::size_t rQmark = line.find('"', ++lQmark);

    return line.substr(lQmark, rQmark - lQmark);
}

bool isBracketsValid(std::size_t l, std::size_t r){
    return  !((l == std::string::npos) ||
              (r == std::string::npos) ||
              (l > r));
}

bool isFollowingStructure(const std::string & line, std::size_t from=0){
    std::size_t comma   = line.find(',', from);
    std::size_t bracket = line.find('{', from);

    return (bracket < comma);
}

bool convertStrToBool(std::string line){
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

std::string findingStucture(const std::string & line, std::size_t & cursor){
    std::size_t lBracket = line.find('{', cursor);
    std::size_t rBracket = line.find('}', cursor);

    std::size_t rigidL = lBracket;
    std::size_t rigidR = rBracket;

    int count = 1;

    do{
        std::size_t tmpL = line.find('{', ++lBracket);

        while (tmpL < rBracket){
            count++;
            lBracket = tmpL;
            tmpL = line.find('{', ++tmpL);
        }

        if (count > 0){
            rigidR = rBracket;
            rBracket = line.find('}', ++rBracket);
            count--;
        }
    }while (count != 0);

    cursor = ++rigidR;

    return line.substr(rigidL, rigidR - rigidL);
}

std::pair <std::string,std::string>
readingField(const std::string & line, std::size_t & cursor){
    std::string key = readingKey(line, cursor);
    std::string val;

    if (isFollowingStructure(line, ++cursor)){
        val = findingStucture(line, cursor);
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

void Settings::fillingGainControl(std::string line){
    std::map  <std::string, std::string> fields = readingStructure(line);

    std::map<std::string,std::string>::iterator it = fields.find("GainAuto");
    if (it == fields.end()) return;
    GainControl.GainAuto = convertStrToBool(it->second);

    it = fields.find("GainRaw");
    if (it == fields.end()) return;
    GainControl.GainRaw = atoi(it->second.c_str());
}

void Settings::fillingExposure(std::string line){
    std::map  <std::string, std::string> fields = readingStructure(line);

    std::map<std::string,std::string>::iterator it = fields.find("ExposureTimeRaw");
    if (it == fields.end()) return;
    Exposure.ExposureTimeRaw = convertStrToBool(it->second);

    it = fields.find("ExposureTimeRaw");
    if (it == fields.end()) return;
    Exposure.ExposureAuto = atoi(it->second.c_str());
}

void Settings::fillingBalanceWhite(std::string line) {
    std::map  <std::string, std::string> fields = readingStructure(line);

    std::map<std::string,std::string>::iterator it = fields.find("BalanceWhiteRaw");
    if (it == fields.end()) return;
    BalanceWhite.BalanceWhiteRaw = convertStrToBool(it->second);

    it = fields.find("ExposureTimeRaw");
    if (it == fields.end()) return;
    Exposure.ExposureAuto = atoi(it->second.c_str());
}

void Settings::fillingSettings(std::map<std::string, std::string> & fields){
    std::map<std::string,std::string>::iterator fuckingIterator = fields.find("AcquisitionFrameRateAbs");
    AcquisitionFrameRateAbs = strtod(fuckingIterator->second.c_str(), NULL);

    fuckingIterator = fields.find("FlashOn");
    FlashOn = convertStrToBool(fuckingIterator->second);

    fuckingIterator = fields.find("PixelFormat");
    PixelFormat = removingQmarks(fuckingIterator->second);

    fuckingIterator = fields.find("GainControl");
    fillingGainControl(fuckingIterator->second);

    fuckingIterator = fields.find("Exposure");
    fillingExposure(fuckingIterator->second);

    std::cout << AcquisitionFrameRateAbs << " " << FlashOn << " " << PixelFormat << std::endl;
    std::cout << GainControl.GainRaw << " " << GainControl.GainAuto << std::endl;
    std::cout << Exposure.ExposureAuto << " " << Exposure.ExposureTimeRaw << std::endl;
}

void Settings::extract(const char * from){
    std::ifstream file(from, std::ios_base::in);

    if (!isValidFile(file)) return;

    std::string dataLine = removingSpaces(file);
    std::map  <std::string, std::string> fields = readingStructure(dataLine);
    fillingSettings(fields);
    file.close();


    std::cout << dataLine << std::endl;
}

