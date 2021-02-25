#ifndef PSE_HUB_H
#define PSE_HUB_H

#include <map>
#include <string>
#include <vector>

#include "VaccinatieCentrum.h"

using namespace std;

class Hub {
public:

    Hub();

    unsigned int getLeveringenInterval() const;

    void setFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra);

    void setFverbondenCentra(const vector<VaccinatieCentrum *> &fverbondenCentra);

    void setAantalVaccins(unsigned int aantalVaccins);

    void setAantalVaccinsPerLevering(unsigned int aantalVaccinsPerLevering);

    void setLeveringenInterval(unsigned int leveringenInterval);

    void setAantalVaccinsPerLading(unsigned int aantalVaccinsPerLading);

private:
    // const attributes

    // changing attributes
    map<string, VaccinatieCentrum *> fverbonden_centra; // slaagt alle vaccinatie centra op met zoeksleutel: name

    unsigned int aantal_vaccins;
    unsigned int aantal_vaccins_per_levering;
    unsigned int leveringen_interval;
    unsigned int aantal_vaccins_per_lading;

    Hub *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is
};


#endif //PSE_HUB_H
