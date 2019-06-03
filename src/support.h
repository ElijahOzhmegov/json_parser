//
// Created by Elijah Ozhmegov on 2019-05-28.
//

#ifndef JSON_PARSER_SUPPORT_H
#define JSON_PARSER_SUPPORT_H

#include <fstream>
#include <iostream>

#define END_OF_FILE " "

struct Line{
    Line(std::string & line, size_t & beg, size_t & end, size_t & cursor) :
            line(line), beg(beg), end(end), cursor(cursor) {}
    Line(const std::string & line, size_t & beg, size_t & end, size_t & cursor) :
            line(line), beg(beg), end(end), cursor(cursor) {}

    const std::string & line;

    size_t & beg;
    size_t & end;

    size_t & cursor;
};


void remove_spaces(std::string & line);

void extract_value(Line obj);

#endif //JSON_PARSER_SUPPORT_H
