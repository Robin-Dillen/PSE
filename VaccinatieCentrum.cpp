//============================================================================
// Name        : VaccinatieCentrum.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a VaccinatieCentrum
//============================================================================

#include "VaccinatieCentrum.h"
#include "Utils.h"
#include "StatisticsSingleton.h"
#include "Vaccins.h"


VaccinatieCentrum::VaccinatieCentrum(const int kcapaciteit, const int kaantalInwoners,
                                     const string &kfname, const string &kfaddress) : kcapaciteit(kcapaciteit),
                                                                                      kaantal_inwoners(kaantalInwoners),
                                                                                      kfname(kfname),
                                                                                      kfaddress(kfaddress),
                                                                                      _initCheck(this) {
    map<string, int> m;
    aantal_eerste_prikken.push_back(m);
    aantal_geleverde_vaccins_buffer = 0;
    aantal_niet_vaccinaties = kaantal_inwoners;
    aantal_eerste_prikken.resize(1);
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
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return 0;
    return aantal_vaccins.at(type).second;
}

int VaccinatieCentrum::getAantalGeleverdeVaccins(const string &type) const {
    if (aantal_geleverde_vaccins.find(type) == aantal_geleverde_vaccins.end()) return 0;
    return aantal_geleverde_vaccins.at(type);
}

int VaccinatieCentrum::getMaxStock() const {
    return kcapaciteit * 2;
}

Vaccin *const VaccinatieCentrum::getVaccinType(const string &type) {
    if (aantal_vaccins.find(type) == aantal_vaccins.end()) return NULL;
    return aantal_vaccins.at(type).first;
}

int VaccinatieCentrum::getTodaysBatch(const string &type) {
    if (aantal_eerste_prikken.front().find(type) == aantal_eerste_prikken.front().end()) return 0;
    return aantal_eerste_prikken.front().at(type);
}

int VaccinatieCentrum::getTotaalAantalVaccinaties() const {
    int totaalAantalVaccinaties = 0;
    for (map<string, int>::const_iterator it = aantal_vaccinaties.begin(); it != aantal_vaccinaties.end(); it++) {
        totaalAantalVaccinaties += getAantalVaccinaties(it->first);
    }
    return totaalAantalVaccinaties;
}

int VaccinatieCentrum::getTotaalAantalVaccins() const {
    int totaalAantalVaccins = 0;
    for (map<string, pair<Vaccin *, int> >::const_iterator it = aantal_vaccins.begin();
         it != aantal_vaccins.end(); it++) {
        totaalAantalVaccins += getAantalVaccins(it->first);
    }
    return totaalAantalVaccins;
}

int VaccinatieCentrum::getTotaalAantalGeleverdeVaccins() const {
    int totaalAantalGeleverdeVaccins = 0;
    for (map<string, int>::const_iterator it = aantal_geleverde_vaccins.begin();
         it != aantal_geleverde_vaccins.end(); it++) {
        totaalAantalGeleverdeVaccins += getAantalGeleverdeVaccins(it->first);
    }
    return totaalAantalGeleverdeVaccins;
}


void VaccinatieCentrum::setVaccins(int vaccins, const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setVaccins");
    aantal_vaccins[type].second = vaccins;
    ENSURE(vaccins == getAantalVaccins(type), "De vaccins zijn niet succesvol ge-set!");
}

void VaccinatieCentrum::setAantalVaccinaties(int aantalVaccinaties, const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinaties");
    aantal_vaccinaties[type] = aantalVaccinaties;
    ENSURE(aantal_vaccinaties.at(type) == getAantalVaccinaties(type),
           "Het aantal vaccinaties is niet succesvol ge-set!");
}

void VaccinatieCentrum::nieuweDag() {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");

    StatisticsSingleton &stats = StatisticsSingleton::getInstance();

    for (map<string, int>::iterator geleverde_vaccins = aantal_geleverde_vaccins.begin();
         geleverde_vaccins != aantal_geleverde_vaccins.end(); geleverde_vaccins++) {
        aantal_vaccins[geleverde_vaccins->first].second += geleverde_vaccins->second;
        stats.addGeleverdeVaccins(this, geleverde_vaccins->first, geleverde_vaccins->second);
        // reset
        geleverde_vaccins->second = 0;
    }

    int capaciteit = kcapaciteit;
    deque<map<string, int> >::iterator today = aantal_eerste_prikken.begin();
    deque<map<string, int> >::iterator tomorrow = aantal_eerste_prikken.begin() + 1;
    for (map<string, int>::iterator batch = today->begin(); batch != today->end(); batch++) {
        cout << batch->first << endl;
        int min_ = min(3, getAantalVaccins(batch->first), capaciteit, batch->second);
        cout << "Er zijn " << min_ << " aantal 2de prikken met " << batch->first << " gezet!" << endl;
        batch->second -= min_;
        aantal_vaccinaties[batch->first] += min_;
        capaciteit -= min_;
        stats.addVaccinatie(this, batch->first, min_);
        if (batch->second != 0) {
            if (tomorrow->find(batch->first) == tomorrow->end()) {
                std::cout << today->at(batch->first) << endl;
                (*tomorrow)[batch->first] = batch->second;
            } else {
                (*tomorrow)[batch->first] += batch->second;
            }
        }
    }

    for (map<string, pair<Vaccin *, int> >::iterator vaccin = aantal_vaccins.begin();
         vaccin != aantal_vaccins.end() && capaciteit != 0; vaccin++) {
        int aantal_prikken = min(3,capaciteit, vaccin->second.second,aantal_niet_vaccinaties);
        cout << "Er zijn " << aantal_prikken << " 1ste prikken met " << vaccin->first << " gezet!" << endl;
        if (vaccin->second.first->hernieuwing == 0) {
            if (aantal_vaccinaties.find(vaccin->first) == aantal_vaccinaties.end()) {
                aantal_vaccinaties[vaccin->first] = aantal_prikken;
            } else aantal_vaccinaties[vaccin->first] += aantal_prikken;
        } else {
            aantal_eerste_prikken[vaccin->second.first->hernieuwing - 1][vaccin->first] = aantal_prikken;
        }
        aantal_niet_vaccinaties -= aantal_prikken;
        capaciteit -= aantal_prikken;
        vaccin->second.second -= aantal_prikken;
    }
    aantal_eerste_prikken.pop_front();
    aantal_eerste_prikken.resize(aantal_eerste_prikken.size() + 1);
}

bool VaccinatieCentrum::isVol() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVol");
    int totaalAantalVaccins = 0;
    map<string, pair<Vaccin *, int> >::const_iterator it;
    for (it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++) {
        totaalAantalVaccins += aantal_vaccins.at(it->first).second;
    }
    return totaalAantalVaccins == getMaxStock();
}

bool VaccinatieCentrum::isVolNaLevering(int vaccins_in_levering) const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isVolNaLevering");
    int VaccinsNaLevering = 0;
    map<string, pair<Vaccin *, int> >::const_iterator it;
    for (it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++) {
        VaccinsNaLevering += aantal_vaccins.at(it->first).second;
        VaccinsNaLevering += aantal_geleverde_vaccins.at(it->first);
    }
    return VaccinsNaLevering > getMaxStock();
}

void VaccinatieCentrum::ontvangLevering(int vaccins_in_levering, Vaccin *vaccin) {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");

    if (vaccin->hernieuwing > (int) aantal_eerste_prikken.size()) aantal_eerste_prikken.resize(vaccin->hernieuwing);
    if (aantal_vaccins.find(vaccin->type) == aantal_vaccins.end()) {
        aantal_vaccins[vaccin->type].first = vaccin;
        aantal_vaccins[vaccin->type].second = 0;
    }

    int begin_aantal_geleverde_vaccins = getAantalGeleverdeVaccins(vaccin->type);
    aantal_geleverde_vaccins[vaccin->type] += vaccins_in_levering;
    ENSURE(begin_aantal_geleverde_vaccins + vaccins_in_levering == getAantalGeleverdeVaccins(vaccin->type),
           "De vaccins zijn niet succesvol geleverd!");
}

bool VaccinatieCentrum::isIedereenGevaccineerd() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
    int totaalAantalVaccinaties = 0;
    map<string, int>::const_iterator it;
    for (it = aantal_vaccinaties.begin(); it != aantal_vaccinaties.end(); it++) {
        totaalAantalVaccinaties += aantal_vaccinaties.at(it->first);
    }
    return totaalAantalVaccinaties == kaantal_inwoners;
}

int VaccinatieCentrum::getAantalGeleverdeVaccinsBuffer() const {
    REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getAantalGeleverdeVaccinsBuffer");
    return aantal_geleverde_vaccins_buffer;
}

int VaccinatieCentrum::getAantalTweedePrikken(const string &vaccin, int dag) const {
    if ((int) aantal_eerste_prikken.size() <= dag) return 0;
    map<string, int>::const_iterator aantal = aantal_eerste_prikken[dag].find(vaccin);
    if (aantal == aantal_eerste_prikken[dag].end()) return 0;
    return aantal->second;
}




