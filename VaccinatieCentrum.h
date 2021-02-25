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

private:
    // const attributes
    const unsigned int kcapaciteit;
    const unsigned int kaantal_inwoners;

    const string kfname;
    const string kfaddress;

    // changing attributes
    unsigned int vaccins;
    unsigned int aantal_vaccinaties;
    unsigned int aantal_geleverde_vaccins;
};


#endif //PSE_VACCINATIECENTRUM_H
