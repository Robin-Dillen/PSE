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
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /**
     * maakt een output file aan of maakt een output file leeg
     * @param filename file dat aangemaakt/leeggemaakt moet worden (zonder path of extensie)
     */
    static void makeOutputFile(const string &filename);

    /**
     * voegt info toe aan het outputbestand
     * @param h: hub
     * @param y,m,w,d: jaren, maanden, weken en dagen na start simulatie
     * @return void
     */
    static void addToOutputFile(Hub *h, int y, int m, int w, int d, const string &filename);

    /**
     * voegt info toe aan het outputbestand
     * @param h: hub
     * @param days:dagen na start simulatie
     * @return void
     */
    static void addToOutputFile(Hub *h, int days, const string &filename);


private:
    /**
     * zet een datum in int formaat om naar een string
     * @param y,m,w,d: jaren, maanden, weken en dagen
     * @return string: datum in string
     */
    static string dateToString(int y, int m, int w, int d);

    const Output *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is
};


#endif //PSE_OUTPUT_H
