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

class Vaccin;

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
     * geeft terug hoeveel 2de prikken er gezet moeten worden vandaag
     * @param type naam van het vaccin
     * @return aantal 2de prikken
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getTodaysBatch()");
     * \n ENSURE(aantal->second >= 0, "Er moet een negatief aantal inwoners gevaccineert worden!");
     */
    int getTodaysBatch(const string &type);

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

    /**
     * update het aantal_vaccins
     * @param vaccins: int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
     * \n ENSURE(aantal_vaccins_begin + vaccins == getAantalVaccins(), "De vaccins zijn niet succesvol toegevoegt!");
     */
    void setVaccins(int vaccins, const string &type);

    /**
     * update het aantal_vaccinaties
     * @param aantalVaccinaties: int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
     * \n ENSURE(aantal_vaccinaties == getAantalVaccinaties(), "Het aantal vaccinaties is niet succesvol ge-set!");
     */
    void setAantalVaccinaties(int aantalVaccinaties,const string &type);

    /*!
     *start een nieuwe dag
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
     * \n ENSURE(aantal_vaccins <= getMaxStock(), "Error, er zijn te veel vaccins geleverd!");
     * \n ENSURE(begin_aantal_vaccins + getAantalGeleverdeVaccins() == aantal_vaccins, "De vaccinaties zijn niet succesvol ontvangen!");
     * \n ENSURE(getAantalGeleverdeVaccins(geleverde_vaccins->first) == 0, "Het aantal geleverde vaccins is niet succesvol gereset!");
     * \n ENSURE(begin_aantal_vaccins - vaccinaties == getAantalVaccins(), "Het aantal vaccins is niet geüpdate!");
     * \n ENSURE(begin_aantal_vaccinaties + vaccinaties == getAantalVaccinaties(), "Het aantal vaccinaties is niet succesvol geüpdate!");
     */
    void nieuweDag();

    /*!
     * geeft terug of de stock van vaccins vol is
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVol");
     */
    bool isVol() const;

    /*!
     * geeft terug of de stock van vaccins vol is na een nieuwe levering
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVolNaLevering");
     */
    bool isVolNaLevering(int vaccins_in_levering) const;

    /*!
     * ontvangt een levering en plaatst de vaccinaties in de stock
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

    const map<string, int> &getAantalVaccinaties1() const;

    const map<string, int> &getAantalGeleverdeVaccins1() const;

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
    void setVaccinInDialog(const std::string &centrum, const Vaccin* vaccin, int i);

    void changeMainProgressBar(int value);

    void changeVaccinProgressBar(const std::string &centrum,const std::string &vaccin,int value);

    void newDay();

private:

    // const attributes
    const int kcapaciteit;
    const int kaantal_inwoners;

    const string kfname;
    const string kfaddress;

    const VaccinatieCentrum *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    map<string, deque<int> > aantal_eerste_prikken;

    // elke loop getten en verwijderen we front, en loopen we door de batches(van front), we checken of we ze een 2de prik kunnen geven etc...
    // -> voeg toe bij aantal vaccinaties(mss voor statistiche verwerking, ook in aantal vaccinaties alles gescheiden houden)
    // als er nog vaccins over zijn, gaan we een nieuwe batch aanmaken(als hernieuwbaar), we checken of aantal_eerste_prikken.size() >= hernieuwbaar
    // zo ja, voeg nieuwe batch toe op plaats hernieuwbaar
    // zo nee aantal_eerste_prikken.resize(hernieuwbaar) !!niet reserve!!
    // we gebruiken een list omdat we front vaak moeten verwijderen(geeft shifts zoals bij vector)

    // changing attributes
    map<string, pair<Vaccin *, int> > aantal_vaccins; //vaccin: Vaccintype, int: aantal vaccins van dit type
    map<string, deque<int> > nog_te_reserveren_vaccins;

    int aantal_niet_vaccinaties;

    //aantal mensen die nog geen vaccinatie hebben gekregen.

    map<string, int> aantal_vaccinaties;

    // aantal mensen dat gevaccineert is
    // we kunnen gwn een map<vaccin_naam, aantal> bijhouden

    map<string, int> aantal_geleverde_vaccins; // aantal vaccins dat toegevoegd wordt na een levering
};

#endif //PSE_VACCINATIECENTRUM_H