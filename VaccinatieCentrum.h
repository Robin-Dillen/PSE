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

#include "Lib.h"
#include "lib/DesignByContract.h"

using namespace std;

/*
class Batch{
private:
    Vaccin* vaccinType;
    int aantal_inwoners;
};
*/

class VaccinatieCentrum {
public:

    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    VaccinatieCentrum(const int kcapaciteit, const int kaantalInwoners, const string &kfname,
                      const string &kfaddress);

    int getAantalGeleverdeVaccinsBuffer() const;

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
     */
    int getAantalVaccinaties(const string &type) const;

    /**
     * @return geeft het adres van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    const string &getKfaddress() const;

    /**
     * @return geeft het aantal inwoners van de plaats, waar het vaccinatie centrum zich bevind, terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    const int getKaantalInwoners() const;

    /**
     * @return geeft capaciteit van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    const int getKcapaciteit() const;

    /**
     * @return geeft het aantal vaccins van het vaccinatie centrum terug die momenteel beschikbaar zijn
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    int getAantalVaccins(const string &type) const;

    /**
     * @return geeft het de maximum opslag weer van het vaccinatiecentrum
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    int getMaxStock() const;

    /**
     * @return geeft het aantal geleverde vaccinaties weer van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    int getAantalGeleverdeVaccins(const string &type) const;

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
     * \n ENSURE(getAantalGeleverdeVaccins() = 0, "Het aantal geleverde vaccins is niet succesvol gereset!");
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
     * \n ENSURE(begin_aantal_geleverde_vaccins + vaccins_in_levering == getAantalGeleverdeVaccins(), "De vaccins zijn niet succesvol geleverd!");
     */
    void ontvangLevering(int vaccins_in_levering, const string &type);

    /*!
     * Kijkt na of iedereen gevaccineerd is.
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
     */
    bool isIedereenGevaccineerd() const;

    void addVaccinType(Vaccin* v);

private:

    // const attributes
    const int kcapaciteit;
    const int kaantal_inwoners;

    const string kfname;
    const string kfaddress;

    const VaccinatieCentrum *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    deque<map<string,int> > aantal_eerste_prikken;
    // elke loop getten en verwijderen we front, en loopen we door de batches(van front), we checken of we ze een 2de prik kunnen geven etc...
    // -> voeg toe bij aantal vaccinaties(mss voor statistiche verwerking, ook in aantal vaccinaties alles gescheiden houden)
    // als er nog vaccins over zijn, gaan we een nieuwe batch aanmaken(als hernieuwbaar), we checken of aantal_eerste_prikken.size() >= hernieuwbaar
    // zo ja, voeg nieuwe batch toe op plaats hernieuwbaar
    // zo nee aantal_eerste_prikken.resize(hernieuwbaar) !!niet reserve!!
    // we gebruiken een list omdat we front vaak moeten verwijderen(geeft shifts zoals bij vector)

    // changing attributes
    map<string,pair<Vaccin *, int> > aantal_vaccins; //vaccin: Vaccintype, int: aantal vaccins van dit type
    //int aantal_vaccins;

    int aantal_niet_vaccinaties; //aantal mensen die nog geen vaccinatie hebben gekregen.

    map<string, int> aantal_vaccinaties; // aantal mensen dat gevaccineert is
    // we kunnen gwn een map<vaccin_naam, aantal> bijhouden

    map<string, int> aantal_geleverde_vaccins; // aantal vaccins dat toegevoegd wordt na een levering
    int aantal_geleverde_vaccins_buffer; // aantal vaccins dat toegevoegd wordt na een levering(buffer for output)
};


#endif //PSE_VACCINATIECENTRUM_H
