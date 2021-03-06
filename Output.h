//
// Created by Niels on 6/03/2021.
//

#ifndef PSE_OUTPUT_H
#define PSE_OUTPUT_H

#include <string>

#include "Hub.h"

using namespace std;

class Output {
public:
    Output(Hub *h, int y, int m, int w);
    void makeOutputFile(Hub *h, int y, int m, int w);
};


#endif //PSE_OUTPUT_H
