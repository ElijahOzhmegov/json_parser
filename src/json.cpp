//
// Created by Elijah Ozhmegov on 2019-05-24.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "json.h"

Settings::Settings(){
    gain.GainAuto = false;
    gain.GainRaw  = 0;

    exposure.ExposureAuto     = false;
    exposure.ExposureTimeRaw  = 0;

    balance.BalanceWhiteAuto = false;
    balance.BalanceWhiteRaw  = 0;

    AutoTargetValue = 0;
    PixelFormat     = "YUV422";

    resolution.Width  = 1224;
    resolution.Height = 1024;

    AcqusitionFrameRateAbs = 25;
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

std::map<std::string, std::string>
readingField(const std::string & line, std::size_t & cursor){
    std::map<std::string, std::string> field;

    std::string key = readingKey(line, cursor);


    if (isFollowingStructure(line, ++cursor)){
        field[key] = findingStucture(line, cursor);
    }else{
        field[key] = readingVal(line, cursor);
    }

    std::cout << "KEY:" << key << std::endl;
    std::cout << "VAL:" << field[key] << std::endl;

    return field;
}

void readingStructure(const std::string & line, std::size_t cursor=0){
    std::size_t lBracket = line.find ('{');
    std::size_t rBracket = line.rfind('}');

    if (!isBracketsValid(lBracket, rBracket)) return;
    lBracket++;
    rBracket--;

    while(cursor < rBracket)
        std::map<std::string, std::string> field(readingField(line.substr(lBracket, rBracket), cursor));

}

void Settings::extract(const char * from){
    std::ifstream file(from, std::ios_base::in);

    if (!isValidFile(file)) return;

    std::string dataLine = removingSpaces(file);
    readingStructure(dataLine);
    file.close();


    std::cout << dataLine << std::endl;
}

