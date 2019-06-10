#include <iostream>
#include <fstream>
#include <string>

#include "src/json.h"

using namespace std;

int main(){
    const char * filename("../cam_config.json");

    Settings S;
    S.extract(filename);

    return 0;
}






