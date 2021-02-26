#ifndef PSE_VACCINATIESIMULATIE_H
#define PSE_VACCINATIESIMULATIE_H

#include "Hub.h"


class VaccinatieSimulatie {

private:
public:
    VaccinatieSimulatie(Hub *fhub);

    /*!
     * start de simulatie
     */
    void start();

private:
    Hub *fhub;
};


#endif //PSE_VACCINATIESIMULATIE_H
