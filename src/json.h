//
// Created by Elijah Ozhmegov on 2019-05-24.
//

#ifndef JSON_PARSER_JSON_H
#define JSON_PARSER_JSON_H

#include <fstream>
#include <iostream>
#include "support.h"


void read_json(std::ifstream & file, std::string & to_long_line);

void get_data(std::string & line);

#endif //JSON_PARSER_JSON_H
