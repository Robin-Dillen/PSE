//
// Created by Niels on 2/06/2021.
//

#ifndef PSE_VACCINSIMULATIE_H
#define PSE_VACCINSIMULATIE_H

#include "Parser.h"
#include "VaccinatieCentrum.h"
#include "Output.h"
#include "Hub.h"
#include "Utils.h"
#include "StatisticsSingleton.h"
#include "Vaccins.h"
#include "Lib.h"

class VaccinSimulatie {
public:
    VaccinSimulatie(vector<Hub *> &hubs, std::vector<VaccinatieCentrum *> &vaccinatie_centra, const string &testfilename,
    bool c_out = true);

    void start();

private:
    vector<Hub*> hubs;
    vector<VaccinatieCentrum*> vaccinatieCentra;
    string filename1;
    string filename2;

};


#endif //PSE_VACCINSIMULATIE_H
