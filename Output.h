//============================================================================
// Name        : OutputSingleton.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : functions to generate output
//============================================================================

#ifndef PSE_OUTPUT_H
#define PSE_OUTPUT_H

#include <sstream>
#include <string>
#include <fstream>
#include "lib/DesignByContract.h"

using namespace std;

class Hub;

class VaccinatieCentrum;

class OutputSingleton {
public:

    /*!
     * geeft de instance van de OutputSingleton terug
     * @return OutputSingleton&
     */
    static OutputSingleton &getInstance() {
        static OutputSingleton instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /**
     * maakt een output file aan of maakt een output file leeg
     * @param filename file dat aangemaakt/leeggemaakt moet worden (zonder path of extensie)
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling makeOutputFile");
     */
    void makeOutputFile(const string &filename);

    /**
     * voegt info toe aan het outputbestand
     * @param h: hub
     * @param i nummer van de hub
     * @param y,m,w,d: jaren, maanden, weken en dagen na start simulatie
     * @return void
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToOutputFile");
     */
    void addToOutputFile(Hub *h, int i, int y, int m, int w, int d, const string &filename);

    /*!
     * voegt info toe aan het GI outputbestand
     * @param v: vaccinatiecentrum
     * @param filename
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToGIFile");
     */
    void addToGIFile(VaccinatieCentrum *v, const string &filename);

    /**
     * voegt info toe aan het outputbestand
     * @param h: hub
     * @param days:dagen na start simulatie
     * @return void
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToOutputFile");
     */
    void addToOutputFile(Hub *h, int i, int days, const string &filename);

    /**
     * voegt info toe aan het GI outputbestand
     * @param v: vaccinatiecentrum
     * @param days:dagen na start simulatie
     * @param filename: naam van file waarin geschreven wordt
     * @return void
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToOutputFile");
     */
    void addToGIFile(VaccinatieCentrum *v, int days, const string &filename);

    /*!
     * Voegt een datum toe aan het outputbestand (in dagen na start simulatie)
     * @param days
     * @param filename: naam van file waarin geschreven wordt
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToGIFile");
     */
    void addDateToFile(int days, const string &filename) const;

private:

    OutputSingleton() : _initCheck(this) {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    OutputSingleton(OutputSingleton const &); // Don't Implement
    void operator=(OutputSingleton const &); // Don't implement

    /**
     * zet een datum in int formaat om naar een string
     * @param y,m,w,d: jaren, maanden, weken en dagen
     * @return string: datum in string
     */
    static string dateToString(int y, int m, int w, int d);

    OutputSingleton *const _initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is
};


#endif //PSE_OUTPUT_H
