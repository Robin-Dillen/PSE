//
// Created by Niels on 6/03/2021.
//

#ifndef PSE_OUTPUT_H
#define PSE_OUTPUT_H

#include "Hub.h"

#include <sstream>
#include <string>
#include <fstream>
#include "lib/DesignByContract.h"

using namespace std;

class Output {
public:
    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    Output(Hub *h, int y, int m, int w, int d);

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /**
     * Maakt een tekst bestand aan
     * @param h: hub
     * @param y,m,w,d: jaren, maanden, weken en dagen na start simulatie
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra");
     */
    static void makeOutputFile(Hub *h, int y, int m, int w, int d, const string &filename);

    /**
     * Maakt een tekst bestand aan
     * @param h: hub
     * @param days:dagen na start simulatie
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra");
     */
    static void makeOutputFile(Hub *h, int days, const string &filename);


private:
    /**
     * zet een datum in int formaat om naar een string
     * @param y,m,w,d: jaren, maanden, weken en dagen na start simulatie
     * @return string: datum in string
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra");
     */
    static string dateToString(int y, int m, int w, int d);

    const Output *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is
};


#endif //PSE_OUTPUT_H
