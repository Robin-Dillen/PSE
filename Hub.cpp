//
// Created by nibor on 25/02/2021.
//

#include "Hub.h"

Hub::Hub(const vector<VaccinatieCentrum *> &vaccinCentra, unsigned int aantal_vaccins_per_levering,
         unsigned int leveringen_interval,
         unsigned int aantal_vaccins_per_lading) : aantal_vaccins(aantal_vaccins_per_levering),
                                                   aantal_vaccins_per_levering(aantal_vaccins_per_levering),
                                                   leveringen_interval(leveringen_interval),
                                                   aantal_vaccins_per_lading(aantal_vaccins_per_lading) {

    for (int i = 0, end = vaccinCentra.size(); i < end; i++) {
        fverbonden_centra[vaccinCentra[i]->getKfname()] = vaccinCentra[i];
    }


}

unsigned int Hub::getLeveringenInterval() const {
    return leveringen_interval;
}
