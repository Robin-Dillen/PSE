//============================================================================
// Name        : Parser.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : parser for xml files
//============================================================================

#ifndef PSE_PARSER_H
#define PSE_PARSER_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include "TinyXML/tinyxml.h"
#include "lib/DesignByContract.h"


class Hub;

using namespace std;

enum EExeption {
    MISSING_TAG, WRONG_VALUE, UNKNOWN_TAG, UNKNOWN_ELEMENT, DUPLICATE_NAME, WRONG_TAG
};


class Parser {
public:
    /**
    * \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    Parser(const string &filename);

    /**
    * \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    Parser();

    ~Parser();

    /**
     * @return geeft een pointer naar de hub terug
     * \n REQUIRE(this->properlyInitialized(), "Parser wasn't initialized when calling getFhub()");
     */
    vector<Hub *> getFhubs() const;

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /**
     * geeft terug hoeveel keer een bepaalde error is voorgekomen
     * @param error: int van de error code
     * @return int, het aantal voorkomens
     */
    int errorOccured(EExeption error) const;

    static string locationToString(TiXmlElement *el);

private:

    // const attributes

    const Parser *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // attributes

    vector<Hub *> fhubs;
    vector<EExeption> errors;
};


#endif //PSE_PARSER_H
