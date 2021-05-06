//============================================================================
// Name        : Hub.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a hub
//============================================================================

#ifndef PSE_HUB_H
#define PSE_HUB_H

#include <map>
#include <string>
#include <vector>
#include <cmath> // voor ceil
#include <iostream> // for testing
#include <algorithm>
#include <deque>

#include "lib/DesignByContract.h"

using namespace std;

class VaccinatieCentrum;
class Vaccin;

class Hub {
public:

    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
//    Hub(const int kaantal_vaccins_per_levering, const int kleveringen_interval, const int kaantal_vaccins_per_lading);

    Hub(const map<string, Vaccin *> &vaccins);

    Hub();

    /**
     * Verwijdert alle centra van deze hub
     */
    virtual ~Hub();

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /**
     * @param type: naam van het vaccin
     * @return int :geeft terug om de hoeveel dagen een levering Kvaccins binnenkomt
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
     */
    int getAantalVaccins(const string &type) const;

    /*!
     * geeft het totaal aantal Kvaccins terug
     * @return int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccins()");
     */
    int getTotaalAantalVaccins() const;

    /**
     * @param type: naam van het vaccin waarvan we het interval willen weten
     * @return int :geeft terug om de hoeveel dagen een levering Kvaccins binnenkomt
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
     */
    int getLeveringenInterval(const string &type) const;

    /*!
     * @param type: naam van het vaccin
     * @return geeft het aantal Kvaccins per levering terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLevering");
     */
    const int getKaantalVaccinsPerLevering(const string &type) const;

    /*!
     * @param type: naam van het vaccin
     * @return geeft het aantal Kvaccins per lading terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLading");
     */
    const int getKaantalVaccinsPerLading(const string &type) const;

    /*!
     * @return geeft de verbonden centra terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFverbondenCentra");
     */
    const map<string, VaccinatieCentrum *> &getFverbondenCentra() const;

    /**
     * update het aantal Kvaccins
     * @param type: naam van het vaccin
     * @param aantalVaccins: int
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccins");
     * \n ENSURE(aantalVaccins == getAantalVaccins(), "Het setten van het aantal Kvaccins is mislukt!");
     */
    void setAantalVaccins(const string &type, int aantalVaccins);

    /**
     * update de verbonden centra
     * @param fverbondenCentra: een map met als key een string(naam van vaccinatie centrum) en als value een pointer naar een vaccinatie centrum
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra");
     * \n ENSURE(fverbonden_centra.size() == fverbondenCentra.size() + start_size, "De centra zijn niet (volledig) Toegevoegd");
     */
    void addFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra);

    /**
     * voegt centra toe aan de verbonden centra
     * @param fverbondenCentra: een vector met een aantal verbonden centra
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra");
     * \n ENSURE(fverbonden_centra.size() == 1 + start_size, "De centra zijn niet (volledig) Toegevoegd");
     */
    void addFverbondenCentra(const vector<VaccinatieCentrum *> &fverbondenCentra);

    /**
    * voegt een centrum toe aan de verbonden centra
    * @param fverbondenCentra: een vector met een aantal verbonden centra
    * @return void
    * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addCentrum");
    * \n ENSURE(fverbonden_centra.size() == 1 + start_size, "De centra zijn niet (volledig) Toegevoegd");
     */
    void addCentrum(VaccinatieCentrum *centrum);

    /*!
     * @return geeft true terug als iedereen gevaccineerd is, anders geeft de functie false terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
     */
    bool isIedereenGevaccineerd() const;

    /*!
     * start een nieuwe dag, verdeelt de Kvaccins en stuurt een signaal naar de vaccinatiecentra
     * REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
     */
    void nieuweDag();

    /*!
     * ontvangt een levering, voegt het aantal geleverde Kvaccins toe aan het aantal Kvaccins van de hub.
     * @param type: naam van het vaccin
     * @param aantal_vaccins het aantal geleverde Kvaccins
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
     * \n ENSURE(aantal_geleverde_vaccins + begin_aantal_vaccins == getAantalVaccins(), "De Kvaccins werden niet succesvol ontvangen!");
     */
    void ontvangLevering(const string &type, int aantal_vaccins);

    /*!
     * verdeelt de Kvaccins over alle verbonden vaccinatie centra
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
     * \n ENSURE(aantal Kvaccins >= 0, "You can't have less than 0 Kvaccins!");
     * \n ENSURE(vaccins_in_levering >= 0, "Er wordt een negatief aantal vaccins geleverd!");
     * \n ENSURE(totaal_vaccins <= centrum->second->getMaxStock(), "Te veel vaccins!");
     */
    void verdeelVaccins();

    /*!
     * geeft het minimum aantal leveringen terug dat nodig is om de capaciteit van het gegeven vaccinaticentrum te bereiken
     * @param centrum iterator
     * @return het minimum aantal leveringen
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
     */
    int
    minAantalLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &centrum, const Vaccin *vaccin) const;

    /*!
     * geeft map met de types Kvaccins terug
     * @return map<string, Vaccin *>
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getVaccins()");
     */
    map<string, Vaccin *> getVaccins();


    /*!
     * Maakt reservaties voor de centra voor bepaald type vaccin
     * @param type: type van vaccin
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
     * \n REQUIRE(!type.empty(), "Type of vaccin cannot be empty.");
     * \n ENSURE(extra_reservatie[i][it->first][type] + gereserveerde_vaccins[i][it->first][type] <= it->second->getKcapaciteit()*2, "Er zijn teveel vaccins gereserveerd");
     * \n ENSURE(aantal_vaccins[type] >= 0, "Er Zijn teveel vaccins gereserveerd");
     */
    void addReservations(const string &type);

    /*!
     *
     * @param dag: een batch van gereserveerde vaccins van een bepaalde dag
     * @return: totaan aantal vaccins die gereserveerd zijn die dag
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getGereserveerdevaccins()");
     */
    int getGereserveerdevaccins(map<string, int> dag);

private:
    // const attributes
//    const int kaantal_vaccins_per_levering;
//    const int kleveringen_interval;
//    const int kaantal_vaccins_per_lading;
    const map<string, Vaccin *> kvaccins;

    const Hub *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // changing attributes
    map<string, VaccinatieCentrum *> fverbonden_centra; // slaagt alle vaccinatie centra op met zoeksleutel: name

//    int aantal_vaccins; // aantal Kvaccins in de hub
    map<string, int> aantal_vaccins;
    deque<map<string, map<string, int> > > gereserveerde_vaccins;
    deque<map<string, map<string, int> > > extra_reservatie;
};


#endif //PSE_HUB_H