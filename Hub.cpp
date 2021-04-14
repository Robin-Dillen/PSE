//============================================================================
// Name        : Hub.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a hub
//============================================================================

#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "Utils.h"
#include "StatisticsSingleton.h"
#include "Vaccins.h"

//Hub::Hub(const int kaantal_vaccins_per_levering,
//         const int kleveringen_interval, const int kaantal_vaccins_per_lading)
//        : kaantal_vaccins_per_levering(kaantal_vaccins_per_levering), kleveringen_interval(kleveringen_interval+1),
//          kaantal_vaccins_per_lading(kaantal_vaccins_per_lading), _initCheck(this),
//          aantal_vaccins(kaantal_vaccins_per_levering) {
//    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
//}

Hub::Hub(const map<string, Vaccin *> &vaccins) : vaccins(vaccins), _initCheck(this) {
    for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
        aantal_vaccins[vaccin->first] = vaccin->second->levering;
        for (map<string, Vaccin *>::const_iterator vaccin_it = vaccins.begin();
             vaccin_it != vaccins.end(); vaccin_it++) {
            gereserveerd_2de_prik[vaccin_it->first] = 0;
        }
    }
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Hub::isProperlyInitialized() const {
    return _initCheck == this;
}

int Hub::getAantalVaccins(const string &type) const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccins");
    return aantal_vaccins.at(type);
}

int Hub::getTotaalAantalvaccins() const{
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccinaties");
    int totaal = 0;
    //loopt over alle vaccinatiecentra van deze hub om het totale aantal vaccinaties te verkrijgen
    for (map<string, int>::const_iterator it = aantal_vaccins.begin(), end = aantal_vaccins.end();
         it != end; it++) {
        totaal += it->second;
    }
    return totaal;
}

int Hub::getLeveringenInterval(const string &type) const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getLeveringenInterval");
    return vaccins.at(type)->interval;
}

int Hub::getTotaalAantalVaccinaties() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccinaties");
    int totaal = 0;
    //loopt over alle vaccinatiecentra van deze hub om het totale aantal vaccinaties te verkrijgen
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
         it != end; it++) {
        totaal += it->second->getTotaalAantalVaccinaties();
    }
    return totaal;
}

const int Hub::getKaantalVaccinsPerLevering(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLevering");
    return vaccins.at(type)->levering;
}

const int Hub::getKaantalVaccinsPerLading(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLading");
    return vaccins.at(type)->transport;
}

const map<string, VaccinatieCentrum *> &Hub::getFverbondenCentra() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFverbondenCentra");
    return fverbonden_centra;
}

void Hub::setAantalVaccins(const string &type, int aantalVaccins) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccins");
    aantal_vaccins[type] = aantalVaccins;
    ENSURE(aantalVaccins == getAantalVaccins(type), "Het setten van het aantal vaccins is mislukt!");
}

void Hub::addFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra(map)");
    //Voegt centra toe aan de hub
    int start_size = fverbonden_centra.size();
    fverbonden_centra = fverbondenCentra;
    ENSURE(fverbonden_centra.size() == fverbondenCentra.size() + start_size,
           "De centra zijn niet (volledig) Toegevoegd");
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
        if (it->second->getTotaalAantalVaccinaties() != it->second->getKaantalInwoners()) return false;
    }
    return true;
}

void Hub::nieuweDag() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");

    // verdeel de vaccins als er nog op voorraad zijn
    for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
        vaccin->second->tijd_tot_nieuwe_levering =
                (vaccin->second->interval + 1 + (((vaccin->second->tijd_tot_nieuwe_levering - 1) %
                                                  (vaccin->second->interval + 1)))) %
                (vaccin->second->interval + 1); // python modulo (wraps around)
        cout << "aantal dagen tot levering: " << vaccin->second->tijd_tot_nieuwe_levering << endl;
    }

    verdeelVaccins();
}

void Hub::ontvangLevering(const string &type, int aantal_geleverde_vaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
    //voegt geleverde vaccins toe aan de stock
    int begin_aantal_vaccins = getAantalVaccins(type);
    aantal_vaccins[type] += aantal_geleverde_vaccins;
    ENSURE(aantal_geleverde_vaccins + begin_aantal_vaccins == getAantalVaccins(type),
           "De vaccins werden niet succesvol ontvangen!");
}

void Hub::verdeelVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");

    for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
        cout << "Aantal vaccins gereserveerd voor de 2de prik: " << gereserveerd_2de_prik[vaccin->first] << endl;
//        if (vaccin->second->tijd_tot_nieuwe_levering != vaccin->second->interval) continue;
        gereserveerd_2de_prik[vaccin->first] = 0;
        for (map<string, VaccinatieCentrum *>::iterator centrum = fverbonden_centra.begin();
             centrum != fverbonden_centra.end(); centrum++) {
            for (int interval = 0; interval <= vaccin->second->tijd_tot_nieuwe_levering; interval++) {
                gereserveerd_2de_prik[vaccin->first] += centrum->second->getAantalTweedePrikken(vaccin->first,
                                                                                                interval);
            }
        }
    }
    //eerste verdeling om alle eerste_prikken van die dag opnieuw te laten vaccineren.
    for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin();
         centrum != fverbonden_centra.end(); centrum++) {
        int capaciteit = centrum->second->getKcapaciteit();
        for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
            int todays_batch = centrum->second->getTodaysBatch(vaccin->first);

            if (centrum->second->getAantalVaccins(vaccin->first) >= todays_batch || capaciteit == 0)
                continue; // we hebben al genoeg vaccins of capaciteit is gevuld

            int min_ = min(3, capaciteit, todays_batch, aantal_vaccins[vaccin->first]);
            capaciteit -= min_;
            int ladingen = ceil((float) min_ / vaccin->second->transport);
            aantal_vaccins[vaccin->first] -= ladingen * vaccin->second->transport;
            centrum->second->ontvangLevering(ladingen * vaccin->second->transport,
                                             vaccin->second);
            ENSURE(capaciteit >= 0, "Er is iets foutgelopen bij de capaciteit!");
        }
    }
    //tweede verdeling zorgt ervoor dat alle centra met de volle capaciteit kan vaccineren
    for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin();
         centrum != fverbonden_centra.end(); centrum++) {
        int capaciteit = centrum->second->getKcapaciteit() - centrum->second->getTotaalAantalGeleverdeVaccins();
        for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
            if (centrum->second->getTotaalAantalVaccins() >= capaciteit || capaciteit == 0) break;
            if (centrum->second->getTotaalAantalVaccins() + vaccin->second->transport >=
                centrum->second->getMaxStock() || aantal_vaccins[vaccin->first] - gereserveerd_2de_prik[vaccin->first] < 0)
                continue;
            int min_ = min(3, capaciteit, aantal_vaccins[vaccin->first] - gereserveerd_2de_prik[vaccin->first],
                           centrum->second->getMaxStock() - centrum->second->getTotaalAantalVaccins());

            capaciteit -= min_;
            int ladingen = ceil((float) min_ / vaccin->second->transport);
            aantal_vaccins[vaccin->first] -= ladingen * vaccin->second->transport;
            centrum->second->ontvangLevering(ladingen * vaccin->second->transport,
                                             vaccin->second); //stuurt de vaccins naar het centrum
            ENSURE(capaciteit >= 0, "Er is iets foutgelopen bij de capaciteit!");
        }
    }
    //derde verdeling zorgt ervoor dat zoveel mogelijk vaccins kunnen worden uitgedeeld
    for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
        for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin(), end = fverbonden_centra.end();
             centrum != end; centrum++) {
            int capaciteit = centrum->second->getKcapaciteit() - centrum->second->getTotaalAantalGeleverdeVaccins();
            while (centrum->second->getTotaalAantalVaccins() + vaccin->second->transport +
                   centrum->second->getTotaalAantalGeleverdeVaccins() < centrum->second->getMaxStock()) {
                if (aantal_vaccins[vaccin->first] - vaccin->second->transport < 0) break;
                if (vaccin->second->temperatuur < 0 && vaccin->second->transport > capaciteit) break;
                centrum->second->ontvangLevering(vaccin->second->transport, vaccin->second);
                aantal_vaccins[vaccin->first] -= vaccin->second->transport;
                capaciteit -= vaccin->second->transport;
            }
        }
    }
}

int
Hub::minAantalLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &centrum, const Vaccin *vaccin) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
    //Berekent hoeveel ladingen er kunnen geleverd worden om de capaciteit van het centrum te bereiken
    return ceil(((float) max(centrum->second->getKcapaciteit() - centrum->second->getAantalVaccins(vaccin->type), 0) /
                 vaccin->transport));
}

int
Hub::minAantalBatchLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &it, const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
    //Berekent hoeveel ladingen er kunnen geleverd worden om alle personen te vaccineren die een 2de spuit nodig hebben
    return ceil(((float) max(it->second->getTodaysBatch(type) - it->second->getAantalVaccins(type), 0) /
                 it->second->getVaccinType(type)->transport));
}


void Hub::addCentrum(VaccinatieCentrum *centrum) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addCentrum");
    unsigned int og_size = fverbonden_centra.size();
    fverbonden_centra[centrum->getKfname()] = centrum;
    ENSURE(fverbonden_centra.size() == og_size + 1, "Het vaccinatie centrum is niet toegevoegt!");
}

map<string, Vaccin *> Hub::getVaccins() {
    return vaccins;
}

Hub::~Hub() {
//    map<string, VaccinatieCentrum *>::iterator it;
//    for(it = fverbonden_centra.begin(); it != fverbonden_centra.end(); it++){
//        // bad code
//        try {
//            delete it->second;
//        } catch (...){
//
//        }
//    }
}


