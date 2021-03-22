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
    map<string,int> m;
    aantal_eerste_prikken.push_back(m);
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

int VaccinatieCentrum::getAantalVaccinaties(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalVaccinaties");
    return aantal_vaccinaties.at(type);
}

const int VaccinatieCentrum::getKaantalInwoners() const {
    return kaantal_inwoners;
}

const int VaccinatieCentrum::getKcapaciteit() const {
    return kcapaciteit;
}

int VaccinatieCentrum::getAantalVaccins(const string &type) const {
    return aantal_vaccins.at(type).second;
}

int VaccinatieCentrum::getAantalGeleverdeVaccins(const string &type) const {
    return aantal_geleverde_vaccins.at(type);
}

int VaccinatieCentrum::getMaxStock() const {
    return kcapaciteit * 2;
}

Vaccin* VaccinatieCentrum::getVaccinType(const string &type) {
    return aantal_vaccins.at(type).first;
}

int VaccinatieCentrum::getTodaysBatch(const string &type) {
    return aantal_eerste_prikken.front().at(type);
}

int VaccinatieCentrum::getTotaalAantalVaccinaties() const {
    int totaalAantalVaccinaties = 0;
    for(map<string,int>::const_iterator it = aantal_vaccinaties.begin(); it != aantal_vaccinaties.end(); it++){
        totaalAantalVaccinaties+=getAantalVaccinaties(it->first);
    }
    return totaalAantalVaccinaties;
}
int VaccinatieCentrum::getTotaalAantalVaccins() const {
    int totaalAantalVaccins = 0;
    for(map<string,pair<Vaccin*, int> >::const_iterator it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++){
        totaalAantalVaccins+=getAantalVaccins(it->first);
    }
    return totaalAantalVaccins;
}
int VaccinatieCentrum::getTotaalAantalGeleverdeVaccins() const {
    int totaalAantalGeleverdeVaccins = 0;
    for(map<string,int>::const_iterator it = aantal_geleverde_vaccins.begin(); it != aantal_geleverde_vaccins.end(); it++){
        totaalAantalGeleverdeVaccins+=getAantalGeleverdeVaccins(it->first);
    }
    return totaalAantalGeleverdeVaccins;
}


void VaccinatieCentrum::setVaccins(int vaccins, const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
    aantal_vaccins[type].second = vaccins;
    ENSURE(vaccins == getAantalVaccins(type), "De vaccins zijn niet succesvol ge-set!");
}

void VaccinatieCentrum::setAantalVaccinaties(int aantalVaccinaties,const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
    aantal_vaccinaties[type] = aantalVaccinaties;
    ENSURE(aantal_vaccinaties.at(type) == getAantalVaccinaties(type), "Het aantal vaccinaties is niet succesvol ge-set!");
}

void VaccinatieCentrum::nieuweDag() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
    map<string,pair<Vaccin *, int> >::iterator it;
    int capaciteit = kcapaciteit;
    // check hoeveel mensen gevaccineerd kunnen worden
    for(it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++){
        int begin_aantal_vaccins = getAantalVaccins(it->first);
        aantal_vaccins.at(it->first).second += aantal_geleverde_vaccins.at(it->first);
        ENSURE(begin_aantal_vaccins + getAantalGeleverdeVaccins(it->first) == aantal_vaccins.at(it->first).second,"De vaccinaties zijn niet succesvol ontvangen!");
        aantal_geleverde_vaccins.at(it->first) = 0;
        int vaccinaties = min(aantal_vaccins.at(it->first).second, kcapaciteit);
        ENSURE(getAantalGeleverdeVaccins(it->first) == 0, "Het aantal geleverde vaccins is niet succesvol gereset!");
        if(vaccinaties > 0){
            //kijk eerst de batch na voor 2de vaccinatie
            if(aantal_eerste_prikken.front().at(it->first) > 0){
                //Alle wachtende personen vaccineren
                if(vaccinaties >= aantal_eerste_prikken.front().at(it->first)){
                    aantal_vaccins.at(it->first).second -= aantal_eerste_prikken.front().at(it->first);
                    aantal_eerste_prikken.front().at(it->first) = 0;
                    aantal_vaccinaties.at(it->first) += aantal_eerste_prikken.front().at(it->first);
                    aantal_niet_vaccinaties -= aantal_eerste_prikken.front().at(it->first);
                    capaciteit-=aantal_eerste_prikken.front().at(it->first);
                    vaccinaties-=aantal_eerste_prikken.front().at(it->first);
                }
                else{
                    aantal_eerste_prikken.front().at(it->first) -= vaccinaties;
                    aantal_vaccinaties.at(it->first) += vaccinaties;
                    aantal_eerste_prikken[1].at(it->first) +=  aantal_eerste_prikken.front().at(it->first);
                    aantal_eerste_prikken.front().at(it->first) = 0;
                    capaciteit-=vaccinaties;
                    vaccinaties = 0;
                }
            }
            vaccinaties = min(vaccinaties, kaantal_inwoners-aantal_niet_vaccinaties);
            if(aantal_vaccins.at(it->first).second > vaccinaties && vaccinaties > 0){
                aantal_niet_vaccinaties += vaccinaties;
                int i = aantal_eerste_prikken.size();
                if(i < aantal_vaccins.at(it->first).first->hernieuwing){
                    aantal_eerste_prikken.resize(aantal_vaccins.at(it->first).first->hernieuwing);
                }
                aantal_eerste_prikken[aantal_vaccins.at(it->first).first->hernieuwing].at(it->first) +=vaccinaties;
                capaciteit -= vaccinaties;
                vaccinaties = 0;
            }
            ENSURE(aantal_eerste_prikken.front().at(it->first) == 0, "Het 2de maal vaccineren van een groep mensen is niet correct verlopen.");
        }
    }
    aantal_eerste_prikken.pop_front();



    /*
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
    // update het aantal vaccins
    int begin_aantal_vaccins = getAantalVaccins();
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

    begin_aantal_vaccins = getAantalVaccins();
    int begin_aantal_vaccinaties = getAantalVaccinaties();
    // verminder het aantal vaccins en vermeerder het aantal gevaccineerden
    aantal_vaccins -= vaccinaties;
    aantal_vaccinaties += vaccinaties;
    ENSURE(begin_aantal_vaccins - vaccinaties == getAantalVaccins(), "Het aantal vaccins is niet geüpdate!");
    ENSURE(begin_aantal_vaccinaties + vaccinaties == getAantalVaccinaties(),
           "Het aantal vaccinaties is niet succesvol geüpdate!");
           */
}

bool VaccinatieCentrum::isVol() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVol");
    int totaalAantalVaccins = 0;
    map<string,pair<Vaccin *, int> >::const_iterator it;
    for(it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++){
        totaalAantalVaccins+=aantal_vaccins.at(it->first).second;
    }
    return totaalAantalVaccins == getMaxStock();
}

bool VaccinatieCentrum::isVolNaLevering(int vaccins_in_levering) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVolNaLevering");
    int VaccinsNaLevering = 0;
    map<string,pair<Vaccin *, int> >::const_iterator it;
    for(it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++){
        VaccinsNaLevering+=aantal_vaccins.at(it->first).second;
        VaccinsNaLevering+=aantal_geleverde_vaccins.at(it->first);
    }
    return VaccinsNaLevering > getMaxStock();
}

void VaccinatieCentrum::ontvangLevering(int vaccins_in_levering,const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
    int begin_aantal_geleverde_vaccins = getAantalGeleverdeVaccins(type);
    aantal_geleverde_vaccins.at(type) += vaccins_in_levering;
    ENSURE(begin_aantal_geleverde_vaccins + vaccins_in_levering == getAantalGeleverdeVaccins(type),
           "De vaccins zijn niet succesvol geleverd!");
}

bool VaccinatieCentrum::isIedereenGevaccineerd() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
    int totaalAantalVaccinaties = 0;
    map<string,int >::const_iterator it;
    for(it = aantal_vaccinaties.begin(); it != aantal_vaccinaties.end(); it++){
        totaalAantalVaccinaties+=aantal_vaccinaties.at(it->first);
    }
    return totaalAantalVaccinaties == kaantal_inwoners;
}

int VaccinatieCentrum::getAantalGeleverdeVaccinsBuffer() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalGeleverdeVaccinsBuffer");
    return aantal_geleverde_vaccins_buffer;
}

void VaccinatieCentrum::addVaccinType(Vaccin* v){
    map<string,pair<Vaccin *, int> >::iterator it;
    for(it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++){
        if(it->first == v->type){
            return;
        }
    }
    aantal_eerste_prikken[0][v->type] = 0;
    aantal_vaccins[v->type] = make_pair(v,0);
    aantal_geleverde_vaccins[v->type] = 0;
    aantal_vaccinaties[v->type] = 0;
    return;
}



