//============================================================================
// Name        : Lib.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : contains constants
//============================================================================

#ifndef PSE_LIB_H
#define PSE_LIB_H

#include <string>

#define OUTPUT_FILE_LOCATION string("../Outputfiles/")

struct Vaccin {
    Vaccin(const std::string &type, const int levering, const int interval, const int transport, const int hernieuwing,
           const int temperatuur) : type(type), levering(levering), interval(interval), transport(transport),
                                    hernieuwing(hernieuwing), temperatuur(temperatuur),
                                    tijd_tot_nieuwe_levering(interval) {}

    const std::string type;
    const int levering; // geleverd aan hub
    const int interval;
    const int transport; // geleverd aan VaccinatieCentrum
    const int hernieuwing;
    const int temperatuur;
    int tijd_tot_nieuwe_levering;
};

#endif //PSE_LIB_H
