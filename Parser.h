#ifndef PSE_PARSER_H
#define PSE_PARSER_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "TinyXML/tinyxml.h"
#include "lib/DesignByContract.h"
#include "Lib.h"


using namespace std;


class Parser {
public:
    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    Parser(const string &filename);

    void parseFile(const string &filename);

    virtual ~Parser();

    /**
     * @return geeft een pointer naar de hub terug
     * \n REQUIRE(this->properlyInitialized(), "Parser wasn't initialized when calling getFhub()");
     */
    Hub *getFhub() const;

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /**
     * geeft terug hoeveel keer een bepaalde error is voorgekomen
     * @param error: int van de error code
     * @return int, het aantal voorkomens
     */
    int errorOccured(char error) const;

private:

    // const attributes

    const Parser *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // attributes

    Hub *fhub;
    vector<char> errors;
};


#endif //PSE_PARSER_H
