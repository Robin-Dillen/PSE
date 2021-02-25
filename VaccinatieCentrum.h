//
// Created by nibor on 25/02/2021.
//

#ifndef PSE_VACCINATIECENTRUM_H
#define PSE_VACCINATIECENTRUM_H

#include <string>

using namespace std;


class VaccinatieCentrum {
public:
    VaccinatieCentrum(const unsigned int kcapaciteit, const unsigned int kaantalInwoners, const string &kfname,
                      const string &kfaddress);

    const string &getKfname() const;

    const string &getKfaddress() const;

    void setVaccins(unsigned int vaccins);

    void setAantalVaccinaties(unsigned int aantalVaccinaties);

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
    unsigned int vaccins;
    unsigned int aantal_vaccinaties;
    unsigned int aantal_geleverde_vaccins;
};


#endif //PSE_VACCINATIECENTRUM_H
