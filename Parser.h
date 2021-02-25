#ifndef PSE_PARSER_H
#define PSE_PARSER_H

#include <fstream>
#include <string>

#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "TinyXML/tinyxml.h"

using namespace std;


class Parser {
public:
    Parser(const string &filename);

private:
    Hub *hub;
};


#endif //PSE_PARSER_H
