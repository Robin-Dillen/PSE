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
#include <QApplication>
#include <deque>

#include "lib/DesignByContract.h"

using namespace std;

class VaccinatieCentrum;
class Vaccin;

class Hub  : public QObject {
    Q_OBJECT
public:

    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
//    Hub(const int kaantal_vaccins_per_levering, const int kleveringen_interval, const int kaantal_vaccins_per_lading);

    Hub(const map<string, Vaccin *> &vaccins);

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
     * \n ENSURE(kvaccins.at(type)->aantal >= 0, "Er is een negatief aantal vaccins!");
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
    int getKaantalVaccinsPerLevering(const string &type) const;

    /*!
     * @param type: naam van het vaccin
     * @return geeft het aantal Kvaccins per lading terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLading");
     */
    int getKaantalVaccinsPerLading(const string &type) const;

    /*!
     * @return geeft de verbonden centra terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFverbondenCentra");
     */
    const map<string, VaccinatieCentrum *> &getFverbondenCentra() const;

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
     * start een nieuwe dag, verdeelt de vaccins (voor de 2de prik) en stuurt een signaal naar de vaccinatiecentra
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
     * verdeelt alle 2de prikken aan de centra
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelTweedePrikken");
     * \n ENSURE(gereserveerd_2de_prik >= 0, "Er zijn meer vaccins geleverd dan gereserveerd");
     */
    void verdeelTweedePrikken();

    /*!
     * verdeelt alle 1ste prikken aan de centra
     * @emit changeVaccinCount(std::string, int)
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelEerstePrikken");
     * \n ENSURE(gereserveerd_1ste_prik >= 0, "Er zijn meer vaccins geleverd dan gereserveerd");
     */
    void verdeelEerstePrikken();

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

    int getFreeStock(VaccinatieCentrum *centrum, int dag);

    int getAllVaccins(const Vaccin *type);

    void distributeManual(std::string type, int count);

public slots:
    void changeAllVaccinCount();

signals:

    void changeVaccinCount(Hub *h, std::string vaccin, int count);

private:
    const Hub *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is
    map<string, Vaccin *> kvaccins;
    map<string, VaccinatieCentrum *> fverbonden_centra; // slaagt alle vaccinatie centra op met zoeksleutel: name
};


#endif //PSE_HUB_H