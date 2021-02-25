#ifndef PSE_PARSER_H
#define PSE_PARSER_H

#include <fstream>
#include <string>
#include <iostream>

#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "TinyXML/tinyxml.h"
#include "lib/DesignByContract.h"

using namespace std;


class Parser {
public:
    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    Parser(const string &filename);

    /**
     * @return geeft een pointer naar de hub terug
     * \n REQUIRE(this->properlyInitialized(), "Parser wasn't initialized when calling getFhub()");
     */
    Hub *getFhub() const;

private:
    //functions

    bool isProperlyInitialized() const;

    // const attributes

    const Parser *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // attributes

    Hub *fhub;
};


#endif //PSE_PARSER_H
