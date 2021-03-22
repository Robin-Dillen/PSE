//============================================================================
// Name        : Hub.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a hub
//============================================================================

#include "Hub.h"
#include "VaccinatieCentrum.h"

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

void Hub::ontvangLevering(const string &type, int aantal_geleverde_vaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
    //voegt geleverde vaccins toe aan de stock
    int begin_aantal_vaccins = getAantalVaccins(type);
    aantal_vaccins[type] += aantal_geleverde_vaccins;
    ENSURE(aantal_geleverde_vaccins + begin_aantal_vaccins == getAantalVaccins(type),
           "De vaccins werden niet succesvol ontvangen!");
}

void Hub::verdeelVaccins() {
    //TEMPERATUUR = //TODO
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
    //eerste verdeling om alle eerste_prikken van die dag opnieuw te laten vaccineren.
    for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin(), end = fverbonden_centra.end(); centrum != end; centrum++) {
        int capaciteit = centrum->second->getKcapaciteit();
        for(map<string,Vaccin*>::const_iterator vaccintype = vaccins.begin(); vaccintype != vaccins.end(); vaccintype++){
            if (centrum->second->getTodaysBatch(vaccintype->first) == 0 ||capaciteit <= 0) {
                continue;
            }
            if(centrum->second->getTodaysBatch(vaccintype->first) < centrum->second->getAantalVaccins(vaccintype->first)){
                capaciteit -= centrum->second->getTodaysBatch(vaccintype->first);
                continue;
            }
            if(centrum->second->getAantalVaccins(vaccintype->first) > capaciteit && centrum->second->getTodaysBatch(vaccintype->first) > capaciteit){
                capaciteit = 0;
                continue;
            }
            int batchLadingen = minAantalBatchLeveringen(centrum,vaccintype->first); //min aantal leveringen om de volledige batch te vaccineren
            while(batchLadingen * vaccintype->second->transport > aantal_vaccins.at(vaccintype->first)) { //zolang er teveel vaccins voorzien zijn dan er aanwezig zijn, verwijder een levering.
                batchLadingen -= vaccintype->second->transport;
            }
            aantal_vaccins.at(vaccintype->first) -= batchLadingen*vaccintype->second->transport;
            centrum->second->ontvangLevering(batchLadingen*vaccintype->second->transport, vaccintype->first); //stuurt de vaccins naar het centrum
            capaciteit -= batchLadingen*vaccintype->second->transport; //niet volledige levering moet gevaccineerd worden-> dus ook niet volledig van capaciteit aftrekken //TODO
            capaciteit -= aantal_vaccins.at(vaccintype->first); //kan zijn dat niet alle vaccins worden gebruikt dus ook niet van capaciteit aftrekken //TODO
        }
    }
    //tweede verdeling zorgt ervoor dat alle centra voor het capaciteit kan vaccineren
    for (map<string, VaccinatieCentrum *>::const_iterator centrum = fverbonden_centra.begin(), end = fverbonden_centra.end(); centrum != end; centrum++) {
        int capaciteit = centrum->second->getKcapaciteit()-centrum->second->getTotaalAantalGeleverdeVaccins();
        for(map<string,Vaccin*>::const_iterator vaccintype = vaccins.begin(); vaccintype != vaccins.end(); vaccintype++){
            if(centrum->second->getAantalVaccins(vaccintype->first) >= capaciteit || capaciteit <= 0){
                break;
            }
            capaciteit -= centrum->second->getAantalVaccins(vaccintype->first);
            int ladingen = minAantalLeveringen(centrum,vaccintype->first);
            while(ladingen * vaccintype->second->transport > aantal_vaccins.at(vaccintype->first)) { //zolang er teveel vaccins voorzien zijn dan er aanwezig zijn, verwijder een levering.
                ladingen -= vaccintype->second->transport;
            }
            aantal_vaccins.at(vaccintype->first) -= ladingen*vaccintype->second->transport;
            centrum->second->ontvangLevering(ladingen*vaccintype->second->transport, vaccintype->first); //stuurt de vaccins naar het centrum
            capaciteit -= ladingen*vaccintype->second->transport; //niet volledige levering moet gevaccineerd worden-> dus ook niet volledig van capaciteit aftrekken //TODO
            capaciteit -= aantal_vaccins.at(vaccintype->first); //kan zijn dat niet alle vaccins worden gebruikt dus ook niet van capaciteit aftrekken //TODO
        }
    }
    //derde verdeling zorgt ervoor dat zoveel mogelijk vaccins kunnen worden uitgedeeld
    for(map<string,Vaccin*>::const_iterator vaccintype = vaccins.begin(); vaccintype != vaccins.end(); vaccintype++){
        bool change = true;
        while(aantal_vaccins.at(vaccintype->first) > vaccintype->second->transport || change){
            change = false;
            for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end(); it != end; it++) {
                if(aantal_vaccins.at(vaccintype->first) < vaccintype->second->transport || it->second->getAantalVaccins(vaccintype->first) + it->second->getAantalGeleverdeVaccins(vaccintype->first) + vaccintype->second->transport > it->second->getMaxStock()){
                    continue;
                }
                it->second->ontvangLevering(vaccintype->second->transport, vaccintype->first);
                aantal_vaccins.at(vaccintype->first) -= vaccintype->second->transport;
                change = true;
            }
        }
    }



//    // eerste verdeling zorgt ervoor dat alle centra genoeg vaccins hebben om voor 1 dag het maximum aantal personen te vaccineren
//    for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
//         it != end; it++) {
//
//         if(...)
//
//        int ladingen = minAantalLeveringen(
//                it); // berekent het minumum aantal leveringen om de capaciteit te vullen
//
//        if (ladingen * kaantal_vaccins_per_lading > aantal_vaccins) {
//            continue;
//        }
//        it->second->ontvangLevering(ladingen * kaantal_vaccins_per_lading); // stuur vaccins naar het centrum
//        aantal_vaccins -= ladingen * kaantal_vaccins_per_lading; // update aantal vaccins van Hub
//    }
//    // 2de verdeling zorgt ervoor dat de vaccins zo veel als mogelijk verdeelt worden
//    bool change = true;
//    while (aantal_vaccins >= kaantal_vaccins_per_lading && change) {
//        change = false;
//        for (map<string, VaccinatieCentrum *>::const_iterator it = fverbonden_centra.begin(), end = fverbonden_centra.end();
//             it != end; it++) {
//            if (aantal_vaccins < kaantal_vaccins_per_lading ||
//                it->second->getAantalVaccins() + it->second->getAantalGeleverdeVaccins() + kaantal_vaccins_per_lading >
//                it->second->getMaxStock() || it->second->isIedereenGevaccineerd())
//                continue;
//            it->second->ontvangLevering(kaantal_vaccins_per_lading);
//            aantal_vaccins -= kaantal_vaccins_per_lading; // update aantal vaccins van Hub
//            change = true;
//        }
//    }
//    ENSURE(aantal_vaccins >= 0, "You can't have less than 0 vaccins!");
}

int Hub::minAantalLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &it, const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
    //Berekent hoeveel ladingen er kunnen geleverd worden om de capaciteit van het centrum te bereiken
    return ceil(((float) max((int) it->second->getKcapaciteit() - (int) it->second->getAantalVaccins(type), 0) /
    it->second->getVaccinType(type)->transport));
}

int Hub::minAantalBatchLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &it, const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
    //Berekent hoeveel ladingen er kunnen geleverd worden om alle personen te vaccineren die een 2de spuit nodig hebben
    return ceil(((float) max((int) it->second->getTodaysBatch(type) - (int) it->second->getAantalVaccins(type), 0) /
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
    map<string, VaccinatieCentrum *>::iterator it;
    for(it = fverbonden_centra.begin(); it != fverbonden_centra.end(); it++){
        delete it->second;
    }
}


