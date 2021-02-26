//
// Created by nibor on 25/02/2021.
//

#ifndef PSE_VACCINATIECENTRUM_H
#define PSE_VACCINATIECENTRUM_H

#include <string>

#include "lib/DesignByContract.h"

using namespace std;


class VaccinatieCentrum {
public:

    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    VaccinatieCentrum(const unsigned int kcapaciteit, const unsigned int kaantalInwoners, const string &kfname,
                      const string &kfaddress);

    /**
     * @return geeft de naam van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfname");
     */
    const string &getKfname() const;

    /**
     * @return geeft het aantal gevacineerde mensen terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccinaties");
     */
    unsigned int getAantalVaccinaties() const;

    /**
     * @return geeft het adres van het vaccinatie centrum terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
     */
    const string &getKfaddress() const;

    /**
     * update het aantal_vaccins
     * @param vaccins: unsigned int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
     */
    void setVaccins(unsigned int vaccins);

    /**
     * update het aantal_vaccinaties
     * @param aantalVaccinaties: unsigned int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
     */
    void setAantalVaccinaties(unsigned int aantalVaccinaties);

    /**
     * update het aantal_geleverde_vaccins
     * @param aantalGeleverdeVaccins: unsigned int
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalGeleverdeVaccins");
     */
    void setAantalGeleverdeVaccins(unsigned int aantalGeleverdeVaccins);

private:

    //functions

    bool isProperlyInitialized() const;

    // const attributes
    const unsigned int kcapaciteit;
    const unsigned int kaantal_inwoners;

    const string kfname;
    const string kfaddress;

    const VaccinatieCentrum *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // changing attributes
    unsigned int aantal_vaccins;
public:
    const unsigned int getKaantalInwoners() const;

private:
    unsigned int aantal_vaccinaties; // aantal mensen dat gevaccineert is
    unsigned int aantal_geleverde_vaccins; // aantal vaccins dat toegevoegd wordt na een levering
};


#endif //PSE_VACCINATIECENTRUM_H
