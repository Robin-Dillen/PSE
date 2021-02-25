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
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

unsigned int Hub::getLeveringenInterval() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getLeveringenInterval");
    return leveringen_interval;
}

void Hub::setFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setFverbondenCentra(map)");
    fverbonden_centra = fverbondenCentra;
}

void Hub::setFverbondenCentra(const vector<VaccinatieCentrum *> &fverbondenCentra) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setFverbondenCentra(vector)");
    for (int i = 0, end = fverbondenCentra.size(); i < end; i++) {
        fverbonden_centra[fverbondenCentra[i]->getKfname()] = fverbondenCentra[i];
    }
}

void Hub::setAantalVaccins(unsigned int aantalVaccins) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccins");
    aantal_vaccins = aantalVaccins;
}

void Hub::setAantalVaccinsPerLevering(unsigned int aantalVaccinsPerLevering) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinsPerLevering");
    aantal_vaccins_per_levering = aantalVaccinsPerLevering;
}

void Hub::setLeveringenInterval(unsigned int leveringenInterval) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setLeveringenInterval");
    leveringen_interval = leveringenInterval;
}

void Hub::setAantalVaccinsPerLading(unsigned int aantalVaccinsPerLading) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinsPerLading");
    aantal_vaccins_per_lading = aantalVaccinsPerLading;
}

bool Hub::isProperlyInitialized() const {
    return _initCheck == this;
}

unsigned int Hub::totaalAantalVaccinaties() const {
    unsigned int totaal = 0;
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        totaal += it->second->getAantalVaccinaties();
    }
    return totaal;
}
