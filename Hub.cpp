//============================================================================
// Name        : Hub.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a hub
//============================================================================

#include "Hub.h"
#include "VaccinatieCentrum.h"

Hub::Hub(const int kaantal_vaccins_per_levering,
         const int kleveringen_interval, const int kaantal_vaccins_per_lading)
        : kaantal_vaccins_per_levering(kaantal_vaccins_per_levering), kleveringen_interval(kleveringen_interval),
          kaantal_vaccins_per_lading(kaantal_vaccins_per_lading), _initCheck(this),
          aantal_vaccins(kaantal_vaccins_per_levering) {
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Hub::isProperlyInitialized() const {
    return _initCheck == this;
}

int Hub::getAantalVaccins() const{
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccins");
    return aantal_vaccins;
}

int Hub::getLeveringenInterval() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getLeveringenInterval");
    return kleveringen_interval;
}

int Hub::getTotaalAantalVaccinaties() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccinaties");
    int totaal = 0;
    //loopt over alle vaccinatiecentra van deze hub om het totale aantal vaccinaties te verkrijgen
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        totaal += it->second->getAantalVaccinaties();
    }
    return totaal;
}

const int Hub::getKaantalVaccinsPerLevering() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLevering");
    return kaantal_vaccins_per_levering;
}

const int Hub::getKaantalVaccinsPerLading() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLading");
    return kaantal_vaccins_per_lading;
}

const map<string, VaccinatieCentrum *> &Hub::getFverbondenCentra() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFverbondenCentra");
    return fverbonden_centra;
}

void Hub::setAantalVaccins(int aantalVaccins) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccins");
    aantal_vaccins = aantalVaccins;
}

void Hub::addFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra(map)");
    //Voegt centra toe aan de hub
    fverbonden_centra = fverbondenCentra;
}

void Hub::addFverbondenCentra(const vector<VaccinatieCentrum *> &fverbondenCentra) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra(vector)");
    int start_size = fverbonden_centra.size();
    //Voegt centra toe aan de hub
    for (int i = 0, end = fverbondenCentra.size(); i < end; i++) {
        fverbonden_centra[fverbondenCentra[i]->getKfname()] = fverbondenCentra[i];
    }
    ENSURE(fverbonden_centra.size() == fverbondenCentra.size() + start_size,
           "De centra zijn niet (volledig) Toegevoegd");
}

bool Hub::isIedereenGevaccineerd() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
    //Kijkt na of iedereen in alle centra al gevaccineerd is of niet
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        if (it->second->getAantalVaccinaties() != it->second->getKaantalInwoners()) return false;
    }
    return true;
}

void Hub::nieuweDag() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
    // verdeel de vaccins als er nog op voorraad zijn
    verdeelVaccins();

    // stuur signaal nieuwe dag naar alle centra
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        /*cout << "Er werden " << it->second->getAantalGeleverdeVaccins() / kaantal_vaccins_per_lading << " ladingen ("
             << it->second->getAantalGeleverdeVaccins() << " vaccins) getransporteerd naar " << it->first << ". En ";*/
        // best zo weinig mogelijk output op de console
        it->second->nieuweDag();
    }
}

void Hub::ontvangLevering( int aantal_geleverde_vaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
    //voegt geleverde vaccins toe aan de stock
    aantal_vaccins += aantal_geleverde_vaccins;
}

void Hub::verdeelVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
    // eerste verdeling zorgt ervoor dat alle centra genoeg vaccins hebben om voor 1 dag het maximum aantal personen te vaccineren
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {

        if (it->second->getAantalVaccins() >= it->second->getKcapaciteit() || it->second->isIedereenGevaccineerd()) {
            continue;
        }
        int ladingen = minAantalLeveringen(
                it); // berekent het minumum aantal leveringen om de capaciteit te vullen

        if (ladingen * kaantal_vaccins_per_lading > aantal_vaccins) {
            continue;
        }
        it->second->ontvangLevering(ladingen * kaantal_vaccins_per_lading); // stuur vaccins naar het centrum
        aantal_vaccins -= ladingen * kaantal_vaccins_per_lading; // update aantal vaccins van Hub
    }
    // 2de verdeling zorgt ervoor dat de vaccins zo veel als mogelijk verdeelt worden
    bool change = true;
    while (aantal_vaccins >= kaantal_vaccins_per_lading && change) {
        change = false;
        for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
             it != end; it++) {
            if (aantal_vaccins < kaantal_vaccins_per_lading ||
                it->second->getAantalVaccins() + it->second->getAantalGeleverdeVaccins() + kaantal_vaccins_per_lading >
                it->second->getMaxStock() || it->second->isIedereenGevaccineerd())
                continue;
            it->second->ontvangLevering(kaantal_vaccins_per_lading);
            aantal_vaccins -= kaantal_vaccins_per_lading; // update aantal vaccins van Hub
            change = true;
        }
    }
    ENSURE(aantal_vaccins >= 0, "You can't have less than 0 vaccins!");
}

int Hub::minAantalLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &it) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
    //Berekent hoeveel ladingen er kunnen geleverd worden om de capaciteit van het centrum te bereiken
    return ceil(((float) max((int) it->second->getKcapaciteit() - (int) it->second->getAantalVaccins(), 0) /
                 kaantal_vaccins_per_lading));
}

void Hub::addCentrum(VaccinatieCentrum *centrum) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addCentrum");
    unsigned int og_size = fverbonden_centra.size();
    fverbonden_centra[centrum->getKfname()] = centrum;
    ENSURE(fverbonden_centra.size() == og_size + 1, "Het vaccinatie centrum is niet toegevoegt!");
}


