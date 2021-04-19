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
        aantal_vaccins[vaccin->first] = vaccin->second->levering;
        for (map<string, Vaccin *>::const_iterator vaccin_it = vaccins.begin();
             vaccin_it != vaccins.end(); vaccin_it++) {
        }
    }
    map<string, map<string, int> > m;
    gereserveerde_vaccins.push_back(m);
    gereserveerde_vaccins.resize(1);
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
    int interval = getVaccins().at(type)->interval;
    int lading = getVaccins().at(type)->transport;
    if((int) gereserveerde_vaccins.size() < interval) gereserveerde_vaccins.resize(interval);
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
    //2de reservatie zoekt het gemiddelde om alle vaccins te verdelen
    if(aantal_vaccins[type] > lading) {
        for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
             it != end; it++) {
            int vaccins = floor(aantal_vaccins[type] / (lading * fverbonden_centra.size())) * lading;
            while(vaccins > lading){
                for(int i = 0; i < interval; i++){
                    if(vaccins < lading){
                        break;
                    }
                    vaccins -= lading;
                    aantal_vaccins[type] -= lading;
                    gereserveerde_vaccins[i][it->first][type] += lading;
                    it->second->reserveerVaccins(type, i, lading);
                }
            }
        }
    }
}

void Hub::verdeelVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
    for (map<string, VaccinatieCentrum *>::iterator centrum = fverbonden_centra.begin(); centrum != fverbonden_centra.end(); centrum++) {
        map<string, int>  gereserveerd_2de_prik = gereserveerde_vaccins.front()[centrum->first];
        //1ste verdeling
        for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
            //aantal gereserveerde vaccins leveren
            centrum->second->ontvangLevering(gereserveerd_2de_prik[vaccin->first], vaccin->second);
        }
        int totaal_vaccins = centrum->second->getTotaalAantalVaccins() + centrum->second->getTotaalAantalGeleverdeVaccins();
        int capaciteit = centrum->second->getKcapaciteit() - totaal_vaccins;
        if (totaal_vaccins >= capaciteit) continue;
        //2de verdeling
        for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
            if (capaciteit <= 0) break;
            ENSURE(totaal_vaccins <= centrum->second->getMaxStock(), "Te veel vaccins!");

            int min_ = min(4,
                           capaciteit,
                           aantal_vaccins[vaccin->first],
                           centrum->second->getMaxStock() - totaal_vaccins,
                           centrum->second->getAantalNietVaccinaties());

            // we checken of we met het afronden niet te veel Kvaccins leveren, zo ja ronden we naar beneden af(en leveren we dus te weinig Kvaccins)
            int ladingen = ceil((float) min_ / vaccin->second->transport);
            if (ladingen * vaccin->second->transport + totaal_vaccins > centrum->second->getMaxStock() ||
                ladingen * vaccin->second->transport - gereserveerd_2de_prik[vaccin->first] < 0) {
                ladingen = floor((float) min_ / vaccin->second->transport);
            }
            int vaccins_in_levering = ladingen * vaccin->second->transport;
            ENSURE(vaccins_in_levering >= 0, "Er wordt een negatief aantal vaccins geleverd!");
            // we verminderen de capaciteit, aantal Kvaccins en we sturen de Kvaccins op
            capaciteit -= vaccins_in_levering;
            aantal_vaccins[vaccin->first] -= vaccins_in_levering;
            centrum->second->ontvangLevering(vaccins_in_levering, vaccin->second); //stuurt de Kvaccins naar het centrum
        }
    }
    gereserveerde_vaccins.pop_front();

    /*
    map<string, int>  gereserveerd_2de_prik;
    for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
        //if (vaccin->second->tijd_tot_nieuwe_levering != vaccin->second->interval) continue;
        gereserveerd_2de_prik[vaccin->first] = 0;
        for (map<string, VaccinatieCentrum *>::iterator centrum = fverbonden_centra.begin();
             centrum != fverbonden_centra.end(); centrum++) {
            for (int interval = 0; interval <= vaccin->second->tijd_tot_nieuwe_levering; interval++) {
                gereserveerd_2de_prik[vaccin->first] += centrum->second->getAantalTweedePrikken(vaccin->first,
                                                                                                interval);
            }
        }
        cout << "\t\tAantal Kvaccins gereserveerd voor de 2de prik met " << vaccin->first << " : "
             << gereserveerd_2de_prik[vaccin->first] << endl;
    }

    //eerste verdeling om alle eerste_prikken van die dag opnieuw te laten vaccineren.
    for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin();
         centrum != fverbonden_centra.end(); centrum++) {
        int capaciteit = centrum->second->getKcapaciteit();
        for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
            if (capaciteit <= 0) break;
            int todays_batch = centrum->second->getTodaysBatch(vaccin->first);

            if (centrum->second->getAantalVaccins(vaccin->first) >= todays_batch || aantal_vaccins[vaccin->first] == 0)
                continue; // we hebben al genoeg Kvaccins of capaciteit is gevuld

            int min_ = min(4,
                           todays_batch -
                           centrum->second->getAantalVaccins(vaccin->first), // is groter dan 0, zie vorige if statement
                           capaciteit,
                           aantal_vaccins[vaccin->first],
                           centrum->second->getMaxStock() - centrum->second->getTotaalAantalVaccins() -
                           centrum->second->getTotaalAantalGeleverdeVaccins());

            // we checken of we met het afronden niet te veel Kvaccins leveren, zo ja ronden we naar beneden af(en leveren we dus te weinig Kvaccins)
            int ladingen = ceil((float) min_ / vaccin->second->transport);
            if (ladingen * vaccin->second->transport + centrum->second->getTotaalAantalVaccins() +
                centrum->second->getTotaalAantalGeleverdeVaccins() > centrum->second->getMaxStock()) {
                ladingen = floor((float) min_ / vaccin->second->transport);
            }
            // we verminderen de capaciteit, aantal Kvaccins en we sturen de Kvaccins op
            int vaccins_in_levering = ladingen * vaccin->second->transport;
            ENSURE(vaccins_in_levering >= 0, "Er wordt een negatief aantal vaccins geleverd!");

            capaciteit -= vaccins_in_levering;
            aantal_vaccins[vaccin->first] -= vaccins_in_levering;
            centrum->second->ontvangLevering(vaccins_in_levering, vaccin->second);
        }
    }
    //tweede verdeling zorgt ervoor dat alle centra met de volle capaciteit kan vaccineren
    for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin();
         centrum != fverbonden_centra.end(); centrum++) {

        int totaal_vaccins =
                centrum->second->getTotaalAantalVaccins() + centrum->second->getTotaalAantalGeleverdeVaccins();
        int capaciteit = centrum->second->getKcapaciteit() - totaal_vaccins;
        if (totaal_vaccins >= capaciteit) continue;

        for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
            if (capaciteit <= 0) break;

            ENSURE(totaal_vaccins <= centrum->second->getMaxStock(), "Te veel vaccins!");
            if (aantal_vaccins[vaccin->first] - gereserveerd_2de_prik[vaccin->first] <= 0) continue;

            int min_ = min(4,
                           capaciteit,
                           aantal_vaccins[vaccin->first] - gereserveerd_2de_prik[vaccin->first],
                           centrum->second->getMaxStock() - totaal_vaccins,
                           centrum->second->getAantalNietVaccinaties());

            // we checken of we met het afronden niet te veel Kvaccins leveren, zo ja ronden we naar beneden af(en leveren we dus te weinig Kvaccins)
            int ladingen = ceil((float) min_ / vaccin->second->transport);
            if (ladingen * vaccin->second->transport + totaal_vaccins > centrum->second->getMaxStock() ||
                ladingen * vaccin->second->transport - gereserveerd_2de_prik[vaccin->first] < 0) {
                ladingen = floor((float) min_ / vaccin->second->transport);
            }
            int vaccins_in_levering = ladingen * vaccin->second->transport;
            ENSURE(vaccins_in_levering >= 0, "Er wordt een negatief aantal vaccins geleverd!");
            // we verminderen de capaciteit, aantal Kvaccins en we sturen de Kvaccins op
            capaciteit -= vaccins_in_levering;
            aantal_vaccins[vaccin->first] -= vaccins_in_levering;
            centrum->second->ontvangLevering(vaccins_in_levering, vaccin->second); //stuurt de Kvaccins naar het centrum
        }
    }
     */
    //derde verdeling zorgt ervoor dat zoveel mogelijk Kvaccins kunnen worden uitgedeeld
//    for (map<string, Vaccin *>::const_iterator vaccin = Kvaccins.begin(); vaccin != Kvaccins.end(); vaccin++) {
//        for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin(), end = fverbonden_centra.end();
//             centrum != end; centrum++) {
//            int capaciteit = centrum->second->getKcapaciteit() - centrum->second->getTotaalAantalGeleverdeVaccins();
//            while (centrum->second->getTotaalAantalVaccins() + vaccin->second->transport +
//                   centrum->second->getTotaalAantalGeleverdeVaccins() < centrum->second->getMaxStock()) {
//                if (aantal_vaccins[vaccin->first] - vaccin->second->transport < 0) break;
//                if (vaccin->second->temperatuur < 0 && vaccin->second->transport > capaciteit) break;
//                centrum->second->ontvangLevering(vaccin->second->transport, vaccin->second);
//                aantal_vaccins[vaccin->first] -= vaccin->second->transport;
//                capaciteit -= vaccin->second->transport;
//            }
//        }
//    }
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


Hub::~Hub() {}

