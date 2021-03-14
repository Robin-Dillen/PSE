//
// Created by nibor on 25/02/2021.
//

#include "VaccinatieCentrum.h"

VaccinatieCentrum::VaccinatieCentrum(const int kcapaciteit, const int kaantalInwoners,
                                     const string &kfname, const string &kfaddress) : kcapaciteit(kcapaciteit),
                                                                                      kaantal_inwoners(kaantalInwoners),
                                                                                      kfname(kfname),
                                                                                      kfaddress(kfaddress),
                                                                                      _initCheck(this) {
    aantal_vaccins = 0;
    aantal_vaccinaties = 0;
    aantal_geleverde_vaccins = 0;
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

bool VaccinatieCentrum::isProperlyInitialized() const {
    return _initCheck == this;
}

const string &VaccinatieCentrum::getKfname() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfname");
    return kfname;
}

const string &VaccinatieCentrum::getKfaddress() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKfaddress");
    return kfaddress;
}

int VaccinatieCentrum::getAantalVaccinaties() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccinaties");
    return aantal_vaccinaties;
}

const int VaccinatieCentrum::getKaantalInwoners() const {
    return kaantal_inwoners;
}

const int VaccinatieCentrum::getKcapaciteit() const {
    return kcapaciteit;
}

int VaccinatieCentrum::getAantalVaccins() const {
    return aantal_vaccins;
}

int VaccinatieCentrum::getAantalGeleverdeVaccins() const {
    return aantal_geleverde_vaccins;
}

int VaccinatieCentrum::getMaxStock() const {
    return kcapaciteit * 2;
}

void VaccinatieCentrum::setVaccins(int vaccins) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
    aantal_vaccins = vaccins;
}

void VaccinatieCentrum::setAantalVaccinaties(int aantalVaccinaties) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
    aantal_vaccinaties = aantalVaccinaties;
}

void VaccinatieCentrum::nieuweDag() {
    // update het aantal vaccins
    aantal_vaccins += aantal_geleverde_vaccins;
    ENSURE(aantal_vaccins <= getMaxStock(), "Error, er zijn te veel vaccins geleverd!");
    aantal_geleverde_vaccins = 0;

    // check hoeveel mensen gevaccineerd kunnen worden
    int vaccinaties = min(aantal_vaccins, kcapaciteit);
    vaccinaties = min(vaccinaties, kaantal_inwoners - aantal_vaccinaties);

    // verminder het aantal vaccins en vermeerder het aantal gevaccineerden
    aantal_vaccins -= vaccinaties;
    aantal_vaccinaties += vaccinaties;
    //cout << "er werden " << vaccinaties << " inwoners gevaccineerd in " << kfname << ". "
    //    << ((double) aantal_vaccinaties / kaantal_inwoners) * 100 << "% is reeds gevaccineerd" << endl;
}

bool VaccinatieCentrum::isVol() const {
    return aantal_vaccins == getMaxStock();
}

bool VaccinatieCentrum::isVolNaLevering(int vaccins_in_levering) const {
    return (aantal_vaccins + vaccins_in_levering) > getMaxStock();
}

void VaccinatieCentrum::ontvangLevering(int vaccins_in_levering) {
    aantal_geleverde_vaccins += vaccins_in_levering;
}

bool VaccinatieCentrum::isIedereenGevaccineerd() const {
    return aantal_vaccinaties == kaantal_inwoners;
}




