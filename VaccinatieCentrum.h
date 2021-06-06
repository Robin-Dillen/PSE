//============================================================================
// Name        : VaccinatieCentrum.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a VaccinatieCentrum
//============================================================================

#ifndef PSE_VACCINATIECENTRUM_H
#define PSE_VACCINATIECENTRUM_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <list>
#include <deque>
#include <QApplication>
#include <QString>
#include "lib/DesignByContract.h"

using namespace std;

struct Vaccin;

class VaccinatieCentrum : public QObject {
    Q_OBJECT
public:

    /**
     * \n ENSURE(kcapaciteit >= 0, "De capaciteit is negatief!");
     * \n ENSURE(kaantalInwoners >= 0, "het aantal inwoners is negatief!");
     * \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    VaccinatieCentrum(const int kcapaciteit, const int kaantalInwoners, const string &kfname,
                      const string &kfaddress);

    VaccinatieCentrum();

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /**
     * @return geeft de naam van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfname");
     */
    const string &getKfname() const;

    /**
     * @return geeft het aantal gevacineerde mensen terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccinaties");
     * \n REQUIRE(!type.empty(), "Het Vaccin type mag geen lege string zijn!");
     * \n ENSURE(aantal->second >= 0, "Het aantal vaccinaties ligt onder nul!");
     */
    int getAantalVaccinaties(const string &type) const;

    /**
     * @return geeft het adres van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    const string &getKfaddress() const;

    /**
     * @return geeft het aantal inwoners van de plaats, waar het vaccinatie centrum zich bevind, terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalInwoners()");
     * \n ENSURE(kaantal_inwoners >= 0, "Het aantal inwoners is minder dan nul!");
     */
    const int getKaantalInwoners() const;

    /**
     * @return geeft capaciteit van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKcapaciteit()");
     * \n ENSURE(kcapaciteit >= 0, "de capaciteit is kleiner dan nul!");
     */
    const int getKcapaciteit() const;

    /**
     * @return geeft het aantal vaccins van het vaccinatie centrum terug die momenteel beschikbaar zijn
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccins()");
     * \n REQUIRE(!type.empty(), "Het Vaccin type mag geen lege string zijn!");
     * \n ENSURE(aantal->second.second >= 0, "Er is een negatief aantal vaccins!");
     */
    int getAantalVaccins(const string &type) const;

    /**
     * @return geeft het de maximum opslag weer van het vaccinatiecentrum
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getMaxStock()");
     * \n ENSURE(kcapaciteit >= 0, "De capaciteit is negatief!");
     */
    int getMaxStock() const;

    /**
     * @return geeft het aantal geleverde vaccinaties weer van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalGeleverdeVaccins()");
     * \n REQUIRE(!type.empty(), "Het Vaccin type mag geen lege string zijn!");
     * \n ENSURE(aantal->second >= 0, "Het aantal gelverde vaccins is negatief!");
     */
    int getAantalGeleverdeVaccins(const string &type) const;

    /*!
     * geeft het totaal aantal vaccinaties terug
     * @return int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccinaties()");
     * \n ENSURE(totaalAantalVaccinaties >= 0, "Het totaal aantal vaccinaties kan niet negatief zijn!");
     */
    int getTotaalAantalVaccinaties() const;

    /*!
     * geeft het totaal aantal vaccins terug
     * @return int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalVaccins()");
     * \n ENSURE(totaal_vaccins >= 0, "Het totaal aantal vaccins kan niet negatief zijn!");
     */
    int getTotaalAantalVaccins() const;

    /*!
     * geeft het totaal aantal vaccins terug dat vandag geleverd wordt
     * @return int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTotaalAantalGeleverdeVaccins()");
     * \n ENSURE(totaal_geleverde_vaccins >= 0, "Het totaal aantal geleverde vaccins kan niet negatief zijn!");
     */
    int getTotaalAantalGeleverdeVaccins() const;

    /*!
     *start een nieuwe dag
     * @emit void VaccinatieCentrum::changeVaccinProgressBar(const std::string &, const std::string&, int)
     * @emit void VaccinatieCentrum::changeMainProgressBar(int)
     * @emit void VaccinatieCentrum::newDay()
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
     * \n ENSURE(aantal_vaccins <= getMaxStock(), "Error, er zijn te veel vaccins geleverd!");
     * \n ENSURE(begin_aantal_vaccins + getAantalGeleverdeVaccins() == aantal_vaccins, "De vaccinaties zijn niet succesvol ontvangen!");
     * \n ENSURE(getAantalGeleverdeVaccins(geleverde_vaccins->first) == 0, "Het aantal geleverde vaccins is niet succesvol gereset!");
     * \n ENSURE(begin_aantal_vaccins - vaccinaties == getAantalVaccins(), "Het aantal vaccins is niet geüpdate!");
     * \n ENSURE(begin_aantal_vaccinaties + vaccinaties == getAantalVaccinaties(), "Het aantal vaccinaties is niet succesvol geüpdate!");
     */
    void nieuweDag();

    /*!
     * ontvangt een levering en plaatst de vaccinaties in de stock
     * @emit void VaccinatieCentrum::setVaccinInDialog(const std::string&, Vaccin*, int)
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
     * \n REQUIRE(vaccins_in_levering >= 0, "Er is een negatief aantal vaccins geleverd!");
     * \n ENSURE(begin_aantal_geleverde_vaccins + vaccins_in_levering == getAantalGeleverdeVaccins(), "De vaccins zijn niet succesvol geleverd!");
     */
    void ontvangLevering(int vaccins_in_levering, Vaccin *vaccin);

    /*!
     * Kijkt na of iedereen gevaccineerd is.
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
     */
    bool isIedereenGevaccineerd() const;

    /*!
     * geeft terug hoeveel 2de prikken er gezet moeten worden in 'dagen' dagen
     * @param vaccin type van het vaccin
     * @param dagen binnen hoeveel dagen we moeten kijken
     * @return aantal 2de prikken: int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalTweedePrikken");
     * \n REQUIRE(!vaccin.empty(), "Het Vaccin type mag geen lege string zijn!");
     * \n REQUIRE(dag > 0, "De dag moet positief zijn!");
     * \n ENSURE(aantal->second >= 0, "We kunnen niet een negatief aantal 2de prikken hebben!");
     */
    int getAantalTweedePrikken(const string &vaccin, int dagen) const;

    /*!
     * geeft terug hoeveel mensen er nog geen vaccinatie hebben gehad
     * @return int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalNietVaccinaties()");
     * \n ENSURE(aantal_niet_vaccinaties >= 0, "We kunnen niet een negatief aantal niet vaccinaties hebben!");
     */
    int getAantalNietVaccinaties() const;

    /*!
     * geeft het aantal vaccinaties in de orginele datastructuur terug
     * @return const map<string, int> &
     * \n REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling this function");
     */
    const map<string, int> &getAantalVaccinaties1() const;

    /*!
     * geeft het aantal geleverde vaccins terug in de orginele datastructuur terug
     * @return const map<string, int> &
     * \n REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling this function");
     */
    const map<string, int> &getAantalGeleverdeVaccins1() const;

    /*!
     * geeft het aantal eerste prikken terug
     * @return const map<string, deque<int>> &
     * \n REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling this function");
     */
    const map<string, deque<int>> &getAantalEerstePrikken() const;

    /*!
     * geeft terug hoeveel vaccins er nog gereserveerd moeten worden voor een bepaalde dag
     * @return int
     * @param type
     * @param dag
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalNietVaccinaties()");
     * \n REQUIRE(nog_te_reserveren_vaccins[dag][type] >= 0, "Er mag geen negatief aantal te reserveren vaccins zijn");
     * \n REQUIRE(!type.empty(), "Het Vaccin type mag geen lege string zijn!");
     * \n REQUIRE(dag > 0, "De dag moet positief zijn!");
     */
    int getNogTeReserverenVaccins(const string &type, int dag);

    /*!
     * geeft terug hoeveel vaccins er nog gereserveerd moeten worden voor een bepaalde dag
     * @return void
     * @param type
     * @param dag
     * @param vaccins: aantal te reserveren vaccins
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalNietVaccinaties()");
     * \n REQUIRE(!type.empty(), "Het Vaccin type mag geen lege string zijn!");
     * \n REQUIRE(dag > 0, "De dag moet positief zijn!");
     * \n REQUIRE(vaccins > 0, "het aantal vaccins moet positief zijn!");
     * \n ENSURE(nog_te_reserveren_vaccins[dag][type] >= 0, "Er mag geen negatief aantal te reserveren vaccins zijn");
     */
    void reserveerVaccins(const string &type, int dag, int vaccins);

    /*!
     * update het aantal gevaccineerden
     * @param aantal: het aantal vaccins dat gezet moet worden
     * \n REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling zet2dePrikVaccins");
     * \n REQUIRE(!type.empty(), "Het Vaccin type mag geen lege string zijn!");
     * \n REQUIRE(capaciteit >= aantal, "Er kunnen niet meer dan capaciteit aantal vaccins gezet worden!");
     * \n ENSURE( aantal_vaccins[type].second >= 0, "Er zijn te weinig vaccins aanwezig");
     */
    void zet2dePrikVaccins(const string &type, int aantal, int &capaciteit);

    /*!
     * update het aantal niet gevaccineerden
     * @param aantal: het aantal vaccins dat gezet moet worden
     * \n REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling zet1stePrikVaccins");
     * \n ENSURE( aantal_vaccins[type].second >= 0, "Er zijn te weinig vaccins aanwezig");
     */
    void zet1stePrikVaccins(const string &type, int aantal, int &capaciteit);

    /*!
     * geeft het variabele aantal vaccins terug
     * @return map<string, pair<Vaccin *, int>> &
     */
    const map<string, pair<Vaccin *, int>> &getAantalVaccins1() const;

signals:

    /*!
     * voegt een type vaccin toe aan de dialoog
     * @param centrum centrum naam
     * @param vaccin vaccinnaam
     * @param centrumnr nr van het centrum
     */
    void setVaccinInDialog(const std::string &centrum, Vaccin *vaccin, int centrumnr);

    /*!
     * update de main progress bar naar value
     * @param value nieuwe value
     */
    void changeMainProgressBar(int value);

    /*!
     * verandert de vaccin progress bar naar value
     * @param centrum naam van het centrum
     * @param vaccin naam van het vaccin
     * @param value nieuwe waarde
     */
    void changeVaccinProgressBar(const std::string &centrum, const std::string &vaccin, int value);

    /*!
     * update het scherm met nieuwe dag
     */
    void newDay();

private:

    // const attributes
    const int kcapaciteit;
    const int kaantal_inwoners;

    const string kfname;
    const string kfaddress;

    const VaccinatieCentrum *const _initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // changing attributes
    map<string, pair<Vaccin *, int> > aantal_vaccins; //vaccin: Vaccintype, int: aantal vaccins van dit type
    map<string, deque<int> > nog_te_reserveren_vaccins;
    map<string, deque<int> > aantal_eerste_prikken;
    int aantal_niet_vaccinaties;
    map<string, int> aantal_vaccinaties;
    map<string, int> aantal_geleverde_vaccins;
};

#endif //PSE_VACCINATIECENTRUM_H