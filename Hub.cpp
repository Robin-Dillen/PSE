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

Hub::Hub(const map<string, Vaccin *> &vaccins) : Kvaccins(vaccins), _initCheck(this) {
    for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
        for (map<string, Vaccin *>::const_iterator vaccin_it = vaccins.begin();
             vaccin_it != vaccins.end(); vaccin_it++) {
        }
    }
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Hub::isProperlyInitialized() const {
    return _initCheck == this;
}

int Hub::getAantalVaccins(const string &type) const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccins");
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    return aantal_vaccins.at(type);
}

int Hub::getTotaalAantalVaccins() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccinaties");
    int totaal = 0;
    for (map<string, int>::const_iterator it = aantal_vaccins.begin(), end = aantal_vaccins.end();
         it != end; it++) {
        totaal += it->second;
    }
    return totaal;
}

int Hub::getLeveringenInterval(const string &type) const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getLeveringenInterval");
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    return Kvaccins.at(type)->interval;
}

const int Hub::getKaantalVaccinsPerLevering(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLevering");
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    return Kvaccins.at(type)->levering;
}

const int Hub::getKaantalVaccinsPerLading(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLading");
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    return Kvaccins.at(type)->transport;
}

const map<string, VaccinatieCentrum *> &Hub::getFverbondenCentra() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFverbondenCentra");
    return fverbonden_centra;
}

void Hub::setAantalVaccins(const string &type, int aantalVaccins) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccins");
    aantal_vaccins[type] = aantalVaccins;
    ENSURE(aantalVaccins == getAantalVaccins(type), "Het setten van het aantal Kvaccins is mislukt!");
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

    // verdeel de Kvaccins als er nog op voorraad zijn
    for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
        vaccin->second->tijd_tot_nieuwe_levering =
                (vaccin->second->interval + 1 + (((vaccin->second->tijd_tot_nieuwe_levering - 1) %
                                                  (vaccin->second->interval + 1)))) %
                (vaccin->second->interval + 1); // python modulo (wraps around)
        cout << "\t\taantal dagen tot levering van " << vaccin->first << ": "
             << vaccin->second->tijd_tot_nieuwe_levering
             << endl;
    }

    verdeelVaccins();
}

void Hub::ontvangLevering(const string &type, int aantal_geleverde_vaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
    //voegt geleverde Kvaccins toe aan de stock
    int begin_aantal_vaccins = getAantalVaccins(type);
    aantal_vaccins[type] += aantal_geleverde_vaccins;
    ENSURE(aantal_geleverde_vaccins + begin_aantal_vaccins == getAantalVaccins(type),
           "De Kvaccins werden niet succesvol ontvangen!");
    addReservations(type);
}

void Hub::addReservations(const string &type) {
    int interval = Kvaccins.at(type)->interval;
    int lading = Kvaccins.at(type)->transport;
    if((int) gereserveerde_vaccins.size() < interval) gereserveerde_vaccins.resize(interval);
    if((int) extra_reservatie.size() < interval) extra_reservatie.resize(interval);
    //1ste reservatie om alle 2de prikken te voorzien
    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end(); it != end; it++) {
        for(int i = 0; i < interval; i++){
            int vaccins = it->second->getNogTeReserverenVaccins(type, i);
            vaccins = ceil(vaccins/lading)*lading;
            while(vaccins > 0 && aantal_vaccins[type] > 0){
                vaccins -= lading;
                aantal_vaccins[type] -= lading;
                gereserveerde_vaccins[i][it->first][type] += lading;
                it->second->reserveerVaccins(type, i, lading);
            }
        }
    }
    //2de reservatie zoekt het gemiddelde om alle vaccins te verdelen, maar niet meer dan capaciteit
    if(aantal_vaccins[type] > lading) {
        for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(); it != fverbonden_centra.end(); it++) {
            int vaccins = floor(aantal_vaccins[type] / (lading * fverbonden_centra.size())) * lading;
            bool done = false;
            while(vaccins >= lading && !done){
                for(int i = 0; i < interval; i++){
                    int capaciteit = it->second->getKcapaciteit() - getGereserveerdevaccins(gereserveerde_vaccins[i][it->first]);
                    if (vaccins < lading) break;
                    if (extra_reservatie[i][it->first][type] > capaciteit){
                        done = true;
                        continue;
                    }
                    else{
                        done = false;
                    }
                    vaccins -= lading;
                    aantal_vaccins[type] -= lading;
                    extra_reservatie[i][it->first][type] += lading;
                }
            }
        }
    }
}

void Hub::verdeelVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
    for (map<string, VaccinatieCentrum *>::iterator centrum = fverbonden_centra.begin(); centrum != fverbonden_centra.end(); centrum++) {
        map<string, int> gereserveerd_2de_prik;
        map<string, int> gereserveerd_1ste_prik;
        if (gereserveerde_vaccins.empty()) {
            return;
        }
        gereserveerd_2de_prik = gereserveerde_vaccins.front()[centrum->first];
        gereserveerd_1ste_prik = extra_reservatie.front()[centrum->first];
        int totaal_vaccins = centrum->second->getTotaalAantalVaccins() + centrum->second->getTotaalAantalGeleverdeVaccins();
        int capaciteit = centrum->second->getKcapaciteit() - totaal_vaccins;
        int maxStock = centrum ->second->getMaxStock();
        //1ste verdeling
        for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
            //aantal gereserveerde vaccins leveren
            int lading = Kvaccins.at(vaccin->first)->transport;
            while (gereserveerd_2de_prik[vaccin->first] > 0 && totaal_vaccins + lading < maxStock){
                centrum->second->ontvangLevering(lading, vaccin->second);
                gereserveerd_2de_prik[vaccin->first] -= lading;
                capaciteit -= lading;
                totaal_vaccins += lading;
            }
            if(gereserveerd_2de_prik[vaccin->first] > 0){
                aantal_vaccins[vaccin->first] += gereserveerd_2de_prik[vaccin->first];
                gereserveerd_2de_prik[vaccin->first] = 0;
            }
        }
        //2de verdeling
        for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
            //aantal gereserveerde vaccins leveren
            int lading = Kvaccins.at(vaccin->first)->transport;
            while (gereserveerd_1ste_prik[vaccin->first] > 0 && capaciteit > 0 && totaal_vaccins + lading < maxStock){
                centrum->second->ontvangLevering(lading, vaccin->second);
                gereserveerd_1ste_prik[vaccin->first] -= lading;
                capaciteit -= lading;
                totaal_vaccins += lading;
            }
            if(gereserveerd_1ste_prik[vaccin->first] > 0){
                aantal_vaccins[vaccin->first] += gereserveerd_1ste_prik[vaccin->first];
                gereserveerd_1ste_prik[vaccin->first] = 0;
            }
        }
    }
    gereserveerde_vaccins.pop_front();
    extra_reservatie.pop_front();
}

int
Hub::minAantalLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &centrum, const Vaccin *vaccin) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
    //Berekent hoeveel ladingen er kunnen geleverd worden om de capaciteit van het centrum te bereiken
    return ceil(((float) max(centrum->second->getKcapaciteit() - centrum->second->getAantalVaccins(vaccin->type), 0) /
                 vaccin->transport));
}

void Hub::addCentrum(VaccinatieCentrum *centrum) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addCentrum");
    unsigned int og_size = fverbonden_centra.size();
    fverbonden_centra[centrum->getKfname()] = centrum;
    ENSURE(fverbonden_centra.size() == og_size + 1, "Het vaccinatie centrum is niet toegevoegt!");
}

map<string, Vaccin *> Hub::getVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getVaccins()");
    return Kvaccins;
}

int Hub::getGereserveerdevaccins(map<string, int> dag) {
    int vaccins = 0;
    for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
         vaccins += dag[vaccin->first];
    }
    return vaccins;
}

Hub::~Hub() {}

