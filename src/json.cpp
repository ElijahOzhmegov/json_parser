//
// Created by Elijah Ozhmegov on 2019-05-24.
//


#include <fstream>
#include <iostream>
#include "json.h"
#include "support.h"

using namespace std;


void read_json(std::ifstream & file, string & to_long_line){
    string line;

    if (!to_long_line.empty())
        to_long_line.erase(to_long_line.begin(), to_long_line.end());

    while (getline(file, line)){
        remove_spaces(line);
        to_long_line.append(line);
    }

    // clearly, opening and closing brackets can be removed
//    to_long_line.erase(to_long_line.begin());
//    to_long_line.erase(to_long_line.end()-1);
}


std::string get_key(const string & line, size_t & cursor){
    if (line.find('"', cursor) == string::npos)
        return END_OF_FILE;

    size_t beg = line.find('"', cursor);
    size_t end = line.find('"', ++beg);

    cursor = end + 1;

    return line.substr(beg, end - beg);
}


std::string get_value(const string & line, size_t & cursor){
    if (line[cursor] != ':')
        return string("");

    size_t beg = line.find(':', cursor);
    size_t end = line.find(',', ++beg);

    extract_value(Line(line, beg, end, cursor));

    cursor = end + 1;

    return  line.substr(beg, end - beg);
}


void get_data(std::string & line){
    if (!line.empty()){
        size_t cursor = 0;

        cout << "------------------" << endl;

        string key;
        string value;

        while (true){
            key = get_key(line, cursor);
            if (key == END_OF_FILE) break;
            value = get_value(line, cursor);

            cout << key   << endl;
            cout << value << endl;
        }

        cout << "------------------" << endl;

    }
}































