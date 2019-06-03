#include <iostream>
#include <fstream>

#include "src/json.h"

using namespace std;

int main(){
    string line;

    string path = "/Users/elijahozhmegov/CLionProjects/work/json_parser/";

    string filename(path + "cam_config.json");
    ifstream file(filename, ifstream::in);

    if (!file) {
        cout << "Unfortunately, did not manage to open your file\n\n";
        return -1;
    }else{
        cout << "Your file was opened successfully!\n\n";

        read_json(file, line);

        cout << line << endl;
        get_data(line);

        cout << "file ended" << endl;
        file.close();

        return 1;
    }

    return 0;
}
