//
// Created by nibor on 25/02/2021.
//

#ifndef PSE_VACCINATIECENTRUM_H
#define PSE_VACCINATIECENTRUM_H

#include <string>
#include <iostream>

#include "lib/DesignByContract.h"

using namespace std;


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
    int getAantalVaccinaties() const;

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
    int getAantalVaccins() const;

    /**
     * @return geeft het de maximum opslag weer van het vaccinatiecentrum
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    int getMaxStock() const;

    /**
     * @return geeft het aantal geleverde vaccinaties weer van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    int getAantalGeleverdeVaccins() const;

    /**
     * update het aantal_vaccins
     * @param vaccins: int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
     */
    void setVaccins(int vaccins);

    /**
     * update het aantal_vaccinaties
     * @param aantalVaccinaties: int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
     */
    void setAantalVaccinaties(int aantalVaccinaties);

    /*!
     *start een nieuwe dag
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
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
     */
    void ontvangLevering(int vaccins_in_levering);

    /*!
     * Kijkt na of iedereen gevaccineerd is.
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
     */
    bool isIedereenGevaccineerd() const;

private:

    // const attributes
    const int kcapaciteit;
    const int kaantal_inwoners;

    const string kfname;
    const string kfaddress;

    const VaccinatieCentrum *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // changing attributes
    int aantal_vaccins;
    int aantal_vaccinaties; // aantal mensen dat gevaccineert is
    int aantal_geleverde_vaccins; // aantal vaccins dat toegevoegd wordt na een levering
    int aantal_geleverde_vaccins_buffer; // aantal vaccins dat toegevoegd wordt na een levering(buffer for output)
};


#endif //PSE_VACCINATIECENTRUM_H
