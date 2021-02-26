//
// Created by nibor on 25/02/2021.
//

#include "Hub.h"

Hub::Hub(const unsigned int kaantal_vaccins_per_levering,
         const unsigned int kleveringen_interval, const unsigned int kaantal_vaccins_per_lading) : _initCheck(this),
                                                                                                   kaantal_vaccins_per_levering(
                                                                                                           kaantal_vaccins_per_levering),
                                                                                                   kleveringen_interval(
                                                                                                           kleveringen_interval),
                                                                                                   kaantal_vaccins_per_lading(
                                                                                                           kaantal_vaccins_per_lading) {
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

unsigned int Hub::getLeveringenInterval() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getLeveringenInterval");
    return kleveringen_interval;
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

/*
void Hub::setAantalVaccinsPerLevering(unsigned int aantalVaccinsPerLevering) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinsPerLevering");
    kaantal_vaccins_per_levering = aantalVaccinsPerLevering;
}

void Hub::setLeveringenInterval(unsigned int leveringenInterval) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setLeveringenInterval");
    kleveringen_interval = leveringenInterval;
}

void Hub::setAantalVaccinsPerLading(unsigned int aantalVaccinsPerLading) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinsPerLading");
    kaantal_vaccins_per_lading = aantalVaccinsPerLading;
}
*/

bool Hub::isProperlyInitialized() const {
    return _initCheck == this;
}

unsigned int Hub::getTotaalAantalVaccinaties() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccinaties");
    unsigned int totaal = 0;
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        totaal += it->second->getAantalVaccinaties();
    }
    return totaal;
}

bool Hub::isIedereenGevaccineerd() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
    return false;
}

void Hub::nieuweDag() {
    // verdeel de vaccins als er nog op voorraad zijn + output
    // VB Er werden 4 ladingen (8000 vaccins) getransporteerd naar Park Spoor Oost.
    verdeelVaccins();

    // stuur signaal nieuwe dag naar alle centra
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        it->second->nieuweDag();
    }
}

void Hub::ontvangLevering(unsigned int aantal_geleverde_vaccins) {
    aantal_vaccins += aantal_geleverde_vaccins;
}

const unsigned int Hub::getKaantalVaccinsPerLevering() const {
    return kaantal_vaccins_per_levering;
}

void Hub::verdeelVaccins() {
    // eerste verdeling zorgt ervoor dat alle centra genoeg vaccins hebben voor 1 dag
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        if (it->second->getAantalVaccins() >= it->second->getKcapaciteit()) continue;
        for (int ladingen = 1; !it->second->isVolNaLevering((ladingen * kaantal_vaccins_per_lading)) &&
                               aantal_vaccins - (ladingen * kaantal_vaccins_per_lading) > 0; ladingen++) {
            it->second->ontvangLevering(ladingen * kaantal_vaccins_per_lading);
        }
    }
    // 2de verdeling zorgt ervoor dat alle vaccins verdeelt worden
    bool change = true;
    while (aantal_vaccins >= 2000 && change) {
        change = false;
        for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
             it != end; it++) {
            for (int ladingen = 1; !it->second->isVolNaLevering((ladingen * kaantal_vaccins_per_lading)) &&
                                   aantal_vaccins - (ladingen * kaantal_vaccins_per_lading) > 0; ladingen++) {
                it->second->ontvangLevering(ladingen * kaantal_vaccins_per_lading);
                change = true;
            }
        }
    }
}
