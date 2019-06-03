//
// Created by Elijah Ozhmegov on 2019-05-28.
//

#include "support.h"


void remove_spaces(std::string & line){
    int count = 0;

    for(int i = 0; i < line.length(); ++i){
        if (line[i] != ' ') line[count++] = line[i];
    }

    line[count] = '\0';
    line.erase (line.begin() + count, line.end());
}


void extract_value(Line obj){
    int brackets_cnt = 0;
    std::size_t opening_bracket = obj.line.find('{', obj.cursor);
    std::size_t closing_bracket = obj.line.find('}', obj.cursor);

    if (obj.end == std::string::npos) obj.end = closing_bracket;

    if (opening_bracket < obj.end){
        ++brackets_cnt;

        while (brackets_cnt > 0){
            std::size_t next_opening_bracket = obj.line.find('{', ++opening_bracket);

            if (next_opening_bracket > closing_bracket) --brackets_cnt;
            else{
                ++brackets_cnt;
                closing_bracket = obj.line.find('}', ++closing_bracket);
            }

            opening_bracket = next_opening_bracket;

        }

        obj.end = ++closing_bracket;
    }
}
