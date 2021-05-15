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

    //voegt geleverde Kvaccins toe aan de stock
    int begin_aantal_vaccins = getAantalVaccins(type);
    aantal_vaccins[type] += aantal_geleverde_vaccins;
    ENSURE(aantal_geleverde_vaccins + begin_aantal_vaccins == getAantalVaccins(type),
           "De Kvaccins werden niet succesvol ontvangen!");
    addReservations(type);
}

void Hub::addReservations(const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addReservation");
    REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
    int interval = kvaccins.at(type)->interval;
    int lading = kvaccins.at(type)->transport;
    //resize deques om reservaties te kunnen toevoegen
    if ((int) gereserveerde_vaccins.size() < interval) gereserveerde_vaccins.resize(interval);
    if ((int) extra_reservatie.size() < interval) extra_reservatie.resize(interval);
    //1ste reservatie om alle 2de prikken te voorzien
    for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin(), end = fverbonden_centra.end();
         centrum != end; centrum++) {
        //loop over alle dagen tussen vandaag en volgende levering van type vaccin
        for (int i = 0; i < interval; i++) {
            //Aantal 2de prikken voor die dag
            int vaccins = centrum->second->getNogTeReserverenVaccins(type, i);
            if (vaccins == 0) continue;
            vaccins = ceil((float) vaccins / lading) * lading;
            //Zolang er voldoende vaccins zijn, ladingen reserveren
            while (vaccins > 0 && aantal_vaccins[type] > 0) {
                vaccins -= lading;
                //gereserveerde vaccins worden appart opgeslagen
                aantal_vaccins[type] -= lading;
                gereserveerde_vaccins[i][centrum->first][type] += lading;
                centrum->second->reserveerVaccins(type, i, lading);
            }
            ENSURE(extra_reservatie[i][centrum->first][type] + gereserveerde_vaccins[i][centrum->first][type] <=
                   centrum->second->getKcapaciteit() * 2, "Er zijn teveel vaccins gereserveerd");
        }
    }
    ENSURE(aantal_vaccins[type] >= 0, "Er Zijn teveel vaccins gereserveerd");
    //2de reservatie zoekt het gemiddelde om alle vaccins te verdelen, maar niet meer dan capaciteit
    if(aantal_vaccins[type] > lading) {
        for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin();
             centrum != fverbonden_centra.end(); centrum++) {
            int vaccins = floor(aantal_vaccins[type] / (lading * fverbonden_centra.size())) * lading;
            bool done = false;
            //Zolang er voldoende vaccins zijn en niet alle centra gevuld zijn tot capaciteit, ladingen reserveren
            while (vaccins >= lading && !done) {
                //loop over alle dagen vannaf vandaag tot nieuwe levering van type vaccin
                for (int dag = 0; dag < interval && vaccins > lading; dag++) {
                    //Check hoeveel vaccins er nog gereserveerd kunnen worden om aan capaciteit te bekomen die dag, of hoeveel mensen nog gevaccineerd moeten worden
                    int free_capaciteit = getFreeStock(centrum->second, dag);

                    //Als gereserveerde vaccins boven capaciteit komt, geen vaccins meer leveren
                    done = false;
                    if (free_capaciteit <= 0) {
                        done = true;
                        continue;
                    }
                    //lading reserveren voor die dag
                    vaccins -= lading;
                    aantal_vaccins[type] -= lading;
                    extra_reservatie[dag][centrum->first][type] += lading;
                    ENSURE(extra_reservatie[dag][centrum->first][type] +
                           gereserveerde_vaccins[dag][centrum->first][type] <=
                           centrum->second->getKcapaciteit() * 2, "Er zijn teveel vaccins gereserveerd");
                }
            }
        }
        ENSURE(aantal_vaccins[type] >= 0, "Er Zijn teveel vaccins gereserveerd");
    }

}

void Hub::verdeelVaccins() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
    for (map<string, VaccinatieCentrum *>::iterator centrum = fverbonden_centra.begin(); centrum != fverbonden_centra.end(); centrum++) {
        map<string, int> gereserveerd_2de_prik;
        map<string, int> gereserveerd_1ste_prik;
        //Als er geen vaccins gereserveerd zijn, moet er ook niks verdeeld worden
        if (gereserveerde_vaccins.front().empty() && extra_reservatie.front().empty()) {
            return;
        }
        gereserveerd_2de_prik = gereserveerde_vaccins.front()[centrum->first];
        gereserveerd_1ste_prik = extra_reservatie.front()[centrum->first];
        int totaal_vaccins = centrum->second->getTotaalAantalVaccins() + centrum->second->getTotaalAantalGeleverdeVaccins();
        int capaciteit = centrum->second->getKcapaciteit() - totaal_vaccins;
        int maxStock = centrum->second->getMaxStock();
        //1ste verdeling om alle 2de prikken te leveren
        for (map<string, Vaccin *>::const_iterator vaccin = kvaccins.begin(); vaccin != kvaccins.end(); vaccin++) {
            //aantal gereserveerde vaccins leveren
            int lading = kvaccins.at(vaccin->first)->transport;
            //Blijft ladingen leveren tot alle vaccins op zijn, of tot capaciteit is bereikt
            while (gereserveerd_2de_prik[vaccin->first] >= lading && totaal_vaccins + lading <= maxStock) {
                centrum->second->ontvangLevering(lading, vaccin->second);
                gereserveerd_2de_prik[vaccin->first] -= lading;
                capaciteit -= lading;
                totaal_vaccins += lading;
            }
            ENSURE(gereserveerd_2de_prik[vaccin->first] >= 0, "Er zijn meer vaccins geleverd dan gereserveerd");
            //Als er nog gereserveerde vaccins over zijn, mogen die terug in vaccins opgeslagen worden voor later
            if (gereserveerd_2de_prik[vaccin->first] > 0) {
                aantal_vaccins[vaccin->first] += gereserveerd_2de_prik[vaccin->first];
                gereserveerd_2de_prik[vaccin->first] = 0;
            }
        }
        //capaciteit = min(capaciteit, centrum->second->getAantalNietVaccinaties());
        //2de verdeling om capaciteit te bekomen
        for (map<string, Vaccin *>::const_iterator vaccin = kvaccins.begin(); vaccin != kvaccins.end(); vaccin++) {
            //aantal gereserveerde vaccins leveren
            int lading = kvaccins.at(vaccin->first)->transport;
            //int temperatuur = kvaccins.at(vaccin->first)->temperatuur;
            //Blijft ladingen leveren tot alle vaccins op zijn, of tot capaciteit is bereikt
            while (gereserveerd_1ste_prik[vaccin->first] >= lading && capaciteit > 0 && totaal_vaccins + lading <= maxStock){
                centrum->second->ontvangLevering(lading, vaccin->second);
                /*if(temperatuur < 0 && capaciteit < lading){
                    break;
                }
                 */
                gereserveerd_1ste_prik[vaccin->first] -= lading;
                capaciteit -= lading;
                totaal_vaccins += lading;
            }
            ENSURE(gereserveerd_1ste_prik[vaccin->first] >= 0, "Er zijn meer vaccins geleverd dan gereserveerd");
            //Als er nog gereserveerde vaccins over zijn, mogen die terug in vaccins opgeslagen worden voor later
            if(gereserveerd_1ste_prik[vaccin->first] > 0){
                aantal_vaccins[vaccin->first] += gereserveerd_1ste_prik[vaccin->first];
                gereserveerd_1ste_prik[vaccin->first] = 0;
            }
        }
        ENSURE(getGereserveerdevaccins(gereserveerd_1ste_prik) == 0, "Er zitten nog vaccins in een verlopen reservatie");
        ENSURE(getGereserveerdevaccins(gereserveerd_2de_prik) == 0, "Er zitten nog vaccins in een verlopen reservatie");
    }
    gereserveerde_vaccins.pop_front();
    extra_reservatie.pop_front();
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

int Hub::getGereserveerdevaccins(map<string, int> dag) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getGereserveerdevaccins()");
    int vaccins = 0;
    for (map<string, Vaccin *>::const_iterator vaccin = kvaccins.begin(); vaccin != kvaccins.end(); vaccin++) {
        vaccins += dag[vaccin->first];
    }
    return vaccins;
}

Hub::~Hub() {}

int Hub::getFreeStock(VaccinatieCentrum *centrum, int dag) {
    int overschot = 0;
    for (int d = 0; d < dag; d++) {
        int aantal = getGereserveerdevaccins(gereserveerde_vaccins[d][centrum->getKfname()]) +
                     getGereserveerdevaccins(extra_reservatie[d][centrum->getKfname()]) - centrum->getKcapaciteit();
        overschot += aantal;
    }
    int stock = centrum->getKcapaciteit() -
                getGereserveerdevaccins(gereserveerde_vaccins[dag][centrum->getKfname()]) -
                getGereserveerdevaccins(extra_reservatie[dag][centrum->getKfname()]);
    if (overschot <= 0) return stock;
    return stock - overschot;
}
