//
// Created by nibor on 25/02/2021.
//

#include "VaccinatieCentrum.h"

VaccinatieCentrum::VaccinatieCentrum(const unsigned int kcapaciteit, const unsigned int kaantalInwoners,
                                     const string &kfname, const string &kfaddress) : kcapaciteit(kcapaciteit),
                                                                                      kaantal_inwoners(kaantalInwoners),
                                                                                      kfname(kfname),
                                                                                      kfaddress(kfaddress) {}

const string &VaccinatieCentrum::getKfname() const {
    return kfname;
}

const string &VaccinatieCentrum::getKfaddress() const {
    return kfaddress;
}

void VaccinatieCentrum::setVaccins(unsigned int vaccins) {
    VaccinatieCentrum::vaccins = vaccins;
}

void VaccinatieCentrum::setAantalVaccinaties(unsigned int aantalVaccinaties) {
    aantal_vaccinaties = aantalVaccinaties;
}

void VaccinatieCentrum::setAantalGeleverdeVaccins(unsigned int aantalGeleverdeVaccins) {
    aantal_geleverde_vaccins = aantalGeleverdeVaccins;
}

bool VaccinatieCentrum::isProperlyInitialized() const {
    return _initCheck == this;
}
