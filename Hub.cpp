//
// Created by nibor on 25/02/2021.
//

#include "Hub.h"

Hub::Hub() {
    aantal_vaccins = 0;
    aantal_vaccins_per_lading = 0;
    aantal_vaccins_per_levering = 0;
    leveringen_interval = 0;

    _initCheck = this;
}

unsigned int Hub::getLeveringenInterval() const {
    return leveringen_interval;
}

void Hub::setFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra) {
    fverbonden_centra = fverbondenCentra;
}

void Hub::setFverbondenCentra(const vector<VaccinatieCentrum *> &fverbondenCentra) {
    for (int i = 0, end = fverbondenCentra.size(); i < end; i++) {
        fverbonden_centra[fverbondenCentra[i]->getKfname()] = fverbondenCentra[i];
    }
}


void Hub::setAantalVaccins(unsigned int aantalVaccins) {
    aantal_vaccins = aantalVaccins;
}

void Hub::setAantalVaccinsPerLevering(unsigned int aantalVaccinsPerLevering) {
    aantal_vaccins_per_levering = aantalVaccinsPerLevering;
}

void Hub::setLeveringenInterval(unsigned int leveringenInterval) {
    leveringen_interval = leveringenInterval;
}

void Hub::setAantalVaccinsPerLading(unsigned int aantalVaccinsPerLading) {
    aantal_vaccins_per_lading = aantalVaccinsPerLading;
}

bool Hub::isProperlyInitialized() const {
    return _initCheck == this;
}

