//
// Created by nibor on 25/02/2021.
//

#include "VaccinatieCentrum.h"

VaccinatieCentrum::VaccinatieCentrum(const unsigned int kcapaciteit, const unsigned int kaantalInwoners,
                                     const string &kfname, const string &kfaddress) : kcapaciteit(kcapaciteit),
                                                                                      kaantal_inwoners(kaantalInwoners),
                                                                                      kfname(kfname),
                                                                                      kfaddress(kfaddress) {
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

const string &VaccinatieCentrum::getKfname() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfname");
    return kfname;
}

const string &VaccinatieCentrum::getKfaddress() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
    return kfaddress;
}

void VaccinatieCentrum::setVaccins(unsigned int vaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
    aantal_vaccins = vaccins;
}

void VaccinatieCentrum::setAantalVaccinaties(unsigned int aantalVaccinaties) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
    aantal_vaccinaties = aantalVaccinaties;
}

void VaccinatieCentrum::setAantalGeleverdeVaccins(unsigned int aantalGeleverdeVaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalGeleverdeVaccins");
    aantal_geleverde_vaccins = aantalGeleverdeVaccins;
}

unsigned int VaccinatieCentrum::getAantalVaccinaties() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccinaties");
    return aantal_vaccinaties;
}

bool VaccinatieCentrum::isProperlyInitialized() const {
    return _initCheck == this;
}

