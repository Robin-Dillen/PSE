//============================================================================
// Name        : VaccinatieCentrum.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a VaccinatieCentrum
//============================================================================

#include "VaccinatieCentrum.h"

VaccinatieCentrum::VaccinatieCentrum(const int kcapaciteit, const int kaantalInwoners,
                                     const string &kfname, const string &kfaddress) : kcapaciteit(kcapaciteit),
                                                                                      kaantal_inwoners(kaantalInwoners),
                                                                                      kfname(kfname),
                                                                                      kfaddress(kfaddress),
                                                                                      _initCheck(this) {
    aantal_vaccinaties = 0;
    aantal_geleverde_vaccins_buffer = 0;
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

int VaccinatieCentrum::getAantalVaccins(const string &type) const {
    return aantal_vaccins;
}

int VaccinatieCentrum::getAantalGeleverdeVaccins(const string &type) const {
    return aantal_geleverde_vaccins;
}

int VaccinatieCentrum::getMaxStock() const {
    return kcapaciteit * 2;
}

void VaccinatieCentrum::setVaccins(int vaccins,const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
    aantal_vaccins = vaccins;
    ENSURE(vaccins == getAantalVaccins(type), "De vaccins zijn niet succesvol ge-set!");
}

void VaccinatieCentrum::setAantalVaccinaties(int aantalVaccinaties) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
    aantal_vaccinaties = aantalVaccinaties;
    ENSURE(aantal_vaccinaties == getAantalVaccinaties(), "Het aantal vaccinaties is niet succesvol ge-set!");
}

void VaccinatieCentrum::nieuweDag() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
    // update het aantal vaccins
    int begin_aantal_vaccins = getAantalVaccins(type);
    aantal_vaccins += aantal_geleverde_vaccins;
    ENSURE(aantal_vaccins <= getMaxStock(), "Error, er zijn te veel vaccins geleverd!");
    ENSURE(begin_aantal_vaccins + getAantalGeleverdeVaccins(type) == aantal_vaccins,
           "De vaccinaties zijn niet succesvol ontvangen!");
    aantal_geleverde_vaccins_buffer = aantal_geleverde_vaccins;
    aantal_geleverde_vaccins = 0;
    ENSURE(getAantalGeleverdeVaccins() == 0, "Het aantal geleverde vaccins is niet succesvol gereset!");

    // check hoeveel mensen gevaccineerd kunnen worden
    int vaccinaties = min(aantal_vaccins, kcapaciteit);
    vaccinaties = min(vaccinaties, kaantal_inwoners - aantal_vaccinaties);

    begin_aantal_vaccins = getAantalVaccins(type);
    int begin_aantal_vaccinaties = getAantalVaccinaties();
    // verminder het aantal vaccins en vermeerder het aantal gevaccineerden
    aantal_vaccins -= vaccinaties;
    aantal_vaccinaties += vaccinaties;
    ENSURE(begin_aantal_vaccins - vaccinaties == getAantalVaccins(type), "Het aantal vaccins is niet geüpdate!");
    ENSURE(begin_aantal_vaccinaties + vaccinaties == getAantalVaccinaties(),
           "Het aantal vaccinaties is niet succesvol geüpdate!");
}

bool VaccinatieCentrum::isVol() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVol");
    return aantal_vaccins == getMaxStock();
}

bool VaccinatieCentrum::isVolNaLevering(int vaccins_in_levering) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVolNaLevering");
    return (aantal_vaccins + vaccins_in_levering) > getMaxStock();
}

void VaccinatieCentrum::ontvangLevering(int vaccins_in_levering,const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
    int begin_aantal_geleverde_vaccins = getAantalGeleverdeVaccins();
    aantal_geleverde_vaccins += vaccins_in_levering;
    ENSURE(begin_aantal_geleverde_vaccins + vaccins_in_levering == getAantalGeleverdeVaccins(),
           "De vaccins zijn niet succesvol geleverd!");
}

bool VaccinatieCentrum::isIedereenGevaccineerd() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
    return aantal_vaccinaties == kaantal_inwoners;
}

int VaccinatieCentrum::getAantalGeleverdeVaccinsBuffer() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalGeleverdeVaccinsBuffer");
    return aantal_geleverde_vaccins_buffer;
}

void VaccinatieCentrum::addVaccinType(Vaccin* v){
    for(int i =0; i<vaccinTypes.size();i++){
        if(vaccinTypes[i].first->type == v->type){
            return;
        }
    }
    vaccinTypes.push_back(make_pair(v, 0));
    aantal_geleverde_vaccins.push_back(make_pair(v, 0));
    return;
}



