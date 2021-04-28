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

Hub::Hub(const map<string, Vaccin *> &vaccins) : kvaccins(vaccins), _initCheck(this) {
    for (map<string, Vaccin *>::const_iterator vaccin = vaccins.begin(); vaccin != vaccins.end(); vaccin++) {
        ontvangLevering(vaccin->first, vaccin->second->levering);
    }
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

Hub::Hub() : kvaccins(map<string, Vaccin *>()), _initCheck(this) {
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Hub::isProperlyInitialized() const {
    return _initCheck == this;
}

int Hub::getAantalVaccins(const string &type) const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccins");
    REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
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
    ENSURE(totaal >= 0, "Het totaal aantal vaccins is negatief!");
    return totaal;
}

int Hub::getLeveringenInterval(const string &type) const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getLeveringenInterval");
    REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    ENSURE(kvaccins.at(type)->interval >= 0, "Het leveringen interval is negatief!");
    return kvaccins.at(type)->interval;
}

const int Hub::getKaantalVaccinsPerLevering(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLevering");
    REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    ENSURE(kvaccins.at(type)->levering >= 0, "Het aantal vaccins per leveringen is negatief!");
    return kvaccins.at(type)->levering;
}

const int Hub::getKaantalVaccinsPerLading(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLading");
    REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    return kvaccins.at(type)->transport;
}

const map<string, VaccinatieCentrum *> &Hub::getFverbondenCentra() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFverbondenCentra");
    return fverbonden_centra;
}

void Hub::setAantalVaccins(const string &type, int aantalVaccins) {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccins");
    REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
    aantal_vaccins[type] = aantalVaccins;
    ENSURE(aantalVaccins == getAantalVaccins(type), "Het setten van het aantal kvaccins is mislukt!");
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

void Hub::addCentrum(VaccinatieCentrum *centrum) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addCentrum");
    REQUIRE(centrum != NULL, "Het centrum is ongeldig!");

    unsigned int og_size = fverbonden_centra.size();
    fverbonden_centra[centrum->getKfname()] = centrum;
    ENSURE(fverbonden_centra.size() == og_size + 1, "Het vaccinatie centrum is niet toegevoegt!");
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

    // verdeel de kvaccins als er nog op voorraad zijn
    for (map<string, Vaccin *>::const_iterator vaccin = kvaccins.begin(); vaccin != kvaccins.end(); vaccin++) {
        vaccin->second->tijd_tot_nieuwe_levering =
                (vaccin->second->interval + 1 + (((vaccin->second->tijd_tot_nieuwe_levering - 1) %
                                                  (vaccin->second->interval + 1)))) %
                (vaccin->second->interval + 1); // python modulo (wraps around)
//        cout << "\t\taantal dagen tot levering van " << vaccin->first << ": "
//             << vaccin->second->tijd_tot_nieuwe_levering
//             << endl;
    }

    verdeelVaccins();
}

void Hub::ontvangLevering(const string &type, int aantal_geleverde_vaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
    REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
    REQUIRE(aantal_geleverde_vaccins > 0, "Het aantal geleverde Vaccins is negatief/nul");

    //voegt geleverde kvaccins toe aan de stock
    int begin_aantal_vaccins = getAantalVaccins(type);
    aantal_vaccins[type] += aantal_geleverde_vaccins;
    ENSURE(aantal_geleverde_vaccins + begin_aantal_vaccins == getAantalVaccins(type),
           "De kvaccins werden niet succesvol ontvangen!");
}

void Hub::verdeelVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");

    for (map<string, Vaccin *>::const_iterator vaccin = kvaccins.begin(); vaccin != kvaccins.end(); vaccin++) {
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
        int totaal_vaccins =
                centrum->second->getTotaalAantalVaccins() + centrum->second->getTotaalAantalGeleverdeVaccins();
        for (map<string, Vaccin *>::const_iterator vaccin = kvaccins.begin(); vaccin != kvaccins.end(); vaccin++) {
            if (capaciteit <= 0) break;
            int todays_batch = centrum->second->getTodaysBatch(vaccin->first);

            if (centrum->second->getAantalVaccins(vaccin->first) >= todays_batch || aantal_vaccins[vaccin->first] == 0)
                continue; // we hebben al genoeg kvaccin of capaciteit is gevuld

            int min_ = min(4,
                           todays_batch -
                           centrum->second->getAantalVaccins(vaccin->first), // is groter dan 0, zie vorige if statement
                           capaciteit,
                           aantal_vaccins[vaccin->first],
                           centrum->second->getMaxStock() - totaal_vaccins);

            // we checken of we met het afronden niet te veel kvaccin leveren, zo ja ronden we naar beneden af(en leveren we dus te weinig kvaccin)
            int ladingen = floor((float) min_ / vaccin->second->transport);
            if (centrum->second->getAantalNietVaccinaties() == 0 &&
                centrum->second->getMaxStock() - totaal_vaccins - vaccin->second->transport > 0 &&
                aantal_vaccins[vaccin->first] >= vaccin->second->transport)
                ladingen = 1;

            // we verminderen de capaciteit, aantal kvaccins en we sturen de kvaccins op
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

        for (map<string, Vaccin *>::const_iterator vaccin = kvaccins.begin(); vaccin != kvaccins.end(); vaccin++) {
            if (capaciteit <= 0) break;

            ENSURE(totaal_vaccins <= centrum->second->getMaxStock(), "Te veel vaccins!");
            if (aantal_vaccins[vaccin->first] - gereserveerd_2de_prik[vaccin->first] <= 0) continue;

            cout << capaciteit << " " << aantal_vaccins[vaccin->first] << "-" << gereserveerd_2de_prik[vaccin->first]
                 << " " << centrum->second->getMaxStock() << "-" << totaal_vaccins << " "
                 << centrum->second->getAantalNietVaccinaties() << endl;

            int min_ = min(4,
                           capaciteit,
                           aantal_vaccins[vaccin->first] - gereserveerd_2de_prik[vaccin->first],
                           centrum->second->getMaxStock() - totaal_vaccins,
                           centrum->second->getAantalNietVaccinaties());

            // we checken of we met het afronden niet te veel kvaccins leveren, zo ja ronden we naar beneden af(en leveren we dus te weinig kvaccins)
            int ladingen = floor((float) min_ / vaccin->second->transport);
            if ((centrum->second->getKcapaciteit() < vaccin->second->transport ||
                 centrum->second->getAantalNietVaccinaties() < vaccin->second->transport) &&
                centrum->second->getMaxStock() - totaal_vaccins - vaccin->second->transport >= 0 &&
                aantal_vaccins[vaccin->first] >= vaccin->second->transport)
                ladingen = 1;

            int vaccins_in_levering = ladingen * vaccin->second->transport;
            ENSURE(vaccins_in_levering >= 0, "Er wordt een negatief aantal vaccins geleverd!");
            // we verminderen de capaciteit, aantal kvaccins en we sturen de kvaccins op
            capaciteit -= vaccins_in_levering;
            aantal_vaccins[vaccin->first] -= vaccins_in_levering;
            centrum->second->ontvangLevering(vaccins_in_levering, vaccin->second); //stuurt de kvaccins naar het centrum
        }
    }
}

int
Hub::minAantalLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &centrum, const Vaccin *vaccin) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
    REQUIRE(vaccin == NULL, "Vaccin is niet geldig!");
    //Berekent hoeveel ladingen er kunnen geleverd worden om de capaciteit van het centrum te bereiken
    return ceil(((float) max(centrum->second->getKcapaciteit() - centrum->second->getAantalVaccins(vaccin->type), 0) /
                 vaccin->transport));
}

map<string, Vaccin *> Hub::getVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getVaccins()");
    return kvaccins;
}

Hub::~Hub() {}
