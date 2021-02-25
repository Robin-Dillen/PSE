#ifndef PSE_HUB_H
#define PSE_HUB_H

#include <map>
#include <string>
#include <vector>

#include "VaccinatieCentrum.h"

using namespace std;

class Hub {
public:

    Hub(const vector<VaccinatieCentrum *> &vaccinCentra, unsigned int aantal_vaccins_per_levering,
        unsigned int leveringen_interval,
        unsigned int aantal_vaccins_per_lading);

private:
    // const attributes

    // changing attributes
    map<string, VaccinatieCentrum *> fverbonden_centra; // slaagt alle vaccinatie centra op met zoeksleutel: name

    unsigned int aantal_vaccins;
    unsigned int aantal_vaccins_per_levering;
    unsigned int leveringen_interval;
    unsigned int aantal_vaccins_per_lading;
};


#endif //PSE_HUB_H
