//
// Created by Niels on 6/03/2021.
//

#ifndef PSE_OUTPUT_H
#define PSE_OUTPUT_H

#include "Hub.h"

#include <sstream>
#include <string>
#include <fstream>

using namespace std;

using namespace std;

class Output {
public:
    Output(Hub *h, int y, int m, int w, int d);

    static void makeOutputFile(Hub *h, int y, int m, int w, int d);

    static void makeOutputFile(Hub *h, unsigned int days);

private:
    static string dateToString(unsigned int y, unsigned int m, unsigned int w, unsigned int d);
};


#endif //PSE_OUTPUT_H
