//
// Created by nibor on 14/03/2021.
//

#ifndef PSE_LIB_H
#define PSE_LIB_H

#include <sstream>

#define MISSING_TAG 1
#define WRONG_VALUE 2
#define UNKNOWN_TAG 3
#define UNKNOWN_ELEMENT 4
#define NO_CENTRA 5
#define DUPLICATE_NAME 6
#define WRONG_TAG 7

template<typename AType>
string to_string(AType var) {
    string str; // a variable of str data type

    // using the stringstream class to insert an int and
    // extract a string
    stringstream ss;
    ss << var;
    ss >> str;
    return str;
}

#endif //PSE_LIB_H
