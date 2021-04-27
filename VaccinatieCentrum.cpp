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
#include "Lib.h"


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
    ENSURE(kcapaciteit >= 0, "De capaciteit is negatief!");
    ENSURE(kaantalInwoners >= 0, "het aantal inwoners is negatief!");
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

bool VaccinatieCentrum::isProperlyInitialized() const {
    return _initCheck == this;
}

const string &VaccinatieCentrum::getKfname() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getKfname");
    return kfname;
}

const string &VaccinatieCentrum::getKfaddress() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getKfaddress");
    return kfaddress;
}

int VaccinatieCentrum::getAantalVaccinaties(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getAantalVaccinaties");
    MapSICIterator aantal = aantal_vaccinaties.find(type);
    if (aantal == aantal_vaccinaties.end()) return 0;
    ENSURE(aantal->second >= 0, "Het aantal vaccinaties ligt onder nul!");
    return aantal->second;
}

const int VaccinatieCentrum::getKaantalInwoners() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getKaantalInwoners()");
    ENSURE(kaantal_inwoners >= 0, "Het aantal inwoners is kleiner dan nul!");
    return kaantal_inwoners;
}

const int VaccinatieCentrum::getKcapaciteit() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getKcapaciteit()");
    ENSURE(kcapaciteit >= 0, "de capaciteit is kleiner dan nul!");
    return kcapaciteit;
}

int VaccinatieCentrum::getAantalVaccins(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getAantalVaccins()");
    MapSP_VI_CIterator aantal = aantal_vaccins.find(type);
    if (aantal == aantal_vaccins.end()) return 0;
    ENSURE(aantal->second.second >= 0, "Er is een negatief aantal vaccins!");
    return aantal->second.second;
}

int VaccinatieCentrum::getAantalGeleverdeVaccins(const string &type) const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getAantalGeleverdeVaccins()");
    MapSICIterator aantal = aantal_geleverde_vaccins.find(type);
    if (aantal == aantal_geleverde_vaccins.end()) return 0;
    ENSURE(aantal->second >= 0, "Het aantal gelverde vaccins is negatief!");
    return aantal->second;
}

int VaccinatieCentrum::getMaxStock() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getMaxStock()");
    ENSURE(kcapaciteit >= 0, "De capaciteit is negatief!");
    return kcapaciteit * 2;
}

int VaccinatieCentrum::getTodaysBatch(const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getTodaysBatch()");
    MapSIIterator aantal = aantal_eerste_prikken.front().find(type);
    if (aantal == aantal_eerste_prikken.front().end()) return 0;
    ENSURE(aantal->second >= 0, "Er moet een negatief aantal inwoners gevaccineert worden!");
    return aantal->second;
}

int VaccinatieCentrum::getTotaalAantalVaccinaties() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getTotaalAantalVaccinaties()");
    int totaal_vaccinaties = 0;
    for (map<string, int>::const_iterator it = aantal_vaccinaties.begin(); it != aantal_vaccinaties.end(); it++) {
        totaal_vaccinaties += getAantalVaccinaties(it->first);
    }
    ENSURE(totaal_vaccinaties >= 0, "Het totaal aantal vaccinaties kan niet negatief zijn!");
    return totaal_vaccinaties;
}

int VaccinatieCentrum::getTotaalAantalVaccins() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getTotaalAantalVaccins()");
    int totaal_vaccins = 0;
    for (map<string, pair<Vaccin *, int> >::const_iterator it = aantal_vaccins.begin();
         it != aantal_vaccins.end(); it++) {
        totaal_vaccins += getAantalVaccins(it->first);
    }
    ENSURE(totaal_vaccins >= 0, "Het totaal aantal vaccins kan niet negatief zijn!");
    return totaal_vaccins;
}

int VaccinatieCentrum::getTotaalAantalGeleverdeVaccins() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getTotaalAantalGeleverdeVaccins()");
    int totaal_geleverde_vaccins = 0;
    for (map<string, int>::const_iterator it = aantal_geleverde_vaccins.begin();
         it != aantal_geleverde_vaccins.end(); it++) {
        totaal_geleverde_vaccins += getAantalGeleverdeVaccins(it->first);
    }
    ENSURE(totaal_geleverde_vaccins >= 0, "Het totaal aantal geleverde vaccins kan niet negatief zijn!");
    return totaal_geleverde_vaccins;
}


void VaccinatieCentrum::setVaccins(int vaccins, const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling setVaccins");
    aantal_vaccins[type].second = vaccins;
    ENSURE(vaccins == getAantalVaccins(type), "De vaccins zijn niet succesvol ge-set!");
}

void VaccinatieCentrum::setAantalVaccinaties(int aantalVaccinaties, const string &type) {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling setAantalVaccinaties");
    aantal_vaccinaties[type] = aantalVaccinaties;
    ENSURE(aantal_vaccinaties.at(type) == getAantalVaccinaties(type),
           "Het aantal vaccinaties is niet succesvol ge-set!");
}

void VaccinatieCentrum::nieuweDag() {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling nieuweDag");

    StatisticsSingleton &stats = StatisticsSingleton::getInstance();

    int begin_aantal_vaccins = getTotaalAantalVaccins();
    int aantal_vaccinaties_vandaag = 0;
    int begin_aantal_vaccinaties = getTotaalAantalVaccinaties();
    int totaal_aantal_geleverde_vaccins = getTotaalAantalGeleverdeVaccins();

    for (map<string, int>::iterator geleverde_vaccins = aantal_geleverde_vaccins.begin();
         geleverde_vaccins != aantal_geleverde_vaccins.end(); geleverde_vaccins++) {
        aantal_vaccins[geleverde_vaccins->first].second += geleverde_vaccins->second;
        stats.addGeleverdeVaccins(this, geleverde_vaccins->first, geleverde_vaccins->second); // TODO
        // het verzamelen van statistische gegevens mag niet in de klassen zelf gebeuren
        // reset
        geleverde_vaccins->second = 0;
        ENSURE(getAantalGeleverdeVaccins(geleverde_vaccins->first) == 0,
               "Het aantal geleverde vaccins is niet succesvol gereset!");
    }

    ENSURE(begin_aantal_vaccins + totaal_aantal_geleverde_vaccins == getTotaalAantalVaccins(),
           "De vaccinaties zijn niet succesvol ontvangen!");

    ENSURE(getTotaalAantalVaccins() <= getMaxStock(), "Error, er zijn te veel vaccins geleverd!");

    begin_aantal_vaccins = getTotaalAantalVaccins();

    int capaciteit = kcapaciteit;
    deque<map<string, int> >::iterator today = aantal_eerste_prikken.begin();
    deque<map<string, int> >::iterator tomorrow = aantal_eerste_prikken.begin() + 1;
    for (map<string, int>::iterator batch = today->begin(); batch != today->end(); batch++) {
        cout << batch->first << endl;
        int min_ = min(3, getAantalVaccins(batch->first), capaciteit, batch->second);
        cout << "Er zijn " << min_ << " aantal 2de prikken met " << batch->first << " gezet!" << endl;

        zetVaccins(batch->first, min_, capaciteit);
        batch->second -= min_;
        aantal_vaccinaties_vandaag += min_;

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
        int aantal_prikken = min(4, capaciteit, vaccin->second.second, aantal_niet_vaccinaties,
                                 getAantalVaccins(vaccin->first));
        cout << "Er zijn " << aantal_prikken << " 1ste prikken met " << vaccin->first << " gezet!" << endl;
        if (vaccin->second.first->hernieuwing == 0) {
            if (aantal_vaccinaties.find(vaccin->first) == aantal_vaccinaties.end()) {
                aantal_vaccinaties[vaccin->first] = aantal_prikken;
            } else aantal_vaccinaties[vaccin->first] += aantal_prikken;
        } else {
            aantal_eerste_prikken[vaccin->second.first->hernieuwing - 1][vaccin->first] = aantal_prikken;
        }
        zetVaccins(vaccin->first, aantal_prikken, capaciteit);
        aantal_vaccinaties_vandaag += aantal_prikken;
        aantal_niet_vaccinaties -= aantal_prikken;
    }

    ENSURE(begin_aantal_vaccins - aantal_vaccinaties_vandaag == getTotaalAantalVaccins(),
           "Het aantal vaccins is niet geüpdate!");
    ENSURE(begin_aantal_vaccinaties + aantal_vaccinaties_vandaag == getTotaalAantalVaccinaties(),
           "Het aantal vaccinaties is niet succesvol geüpdate!");

    aantal_eerste_prikken.pop_front();
    aantal_eerste_prikken.resize(aantal_eerste_prikken.size() + 1);
}

void VaccinatieCentrum::zetVaccins(const string &type, int aantal, int &capaciteit) {
    aantal_vaccinaties[type] += aantal; // bestaat zeker (wordt aangemaakt bij het ontvangen van een levering)
    aantal_vaccins[type].second -= aantal; // update het aantal beschikbare vaccins
    capaciteit -= aantal;
}

bool VaccinatieCentrum::isVol() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling isVol");
    int totaalAantalVaccins = 0;
    map<string, pair<Vaccin *, int> >::const_iterator it;
    for (it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++) {
        totaalAantalVaccins += aantal_vaccins.at(it->first).second;
    }
    return totaalAantalVaccins == getMaxStock();
}

bool VaccinatieCentrum::isVolNaLevering(int vaccins_in_levering) const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling isVolNaLevering");
    int vaccinsNaLevering = 0;
    map<string, pair<Vaccin *, int> >::const_iterator it;
    for (it = aantal_vaccins.begin(); it != aantal_vaccins.end(); it++) {
        vaccinsNaLevering += aantal_vaccins.at(it->first).second;
        vaccinsNaLevering += aantal_geleverde_vaccins.at(it->first);
    }
    return vaccinsNaLevering > getMaxStock();
}

void VaccinatieCentrum::ontvangLevering(int vaccins_in_levering, Vaccin *vaccin) {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling ontvangLevering");

    if (vaccin->hernieuwing > (int) aantal_eerste_prikken.size()) aantal_eerste_prikken.resize(vaccin->hernieuwing);
    if (aantal_vaccins.find(vaccin->type) == aantal_vaccins.end()) {
        aantal_vaccins[vaccin->type].first = vaccin;
        aantal_vaccins[vaccin->type].second = 0;
        aantal_vaccinaties[vaccin->type] = 0;
    }

    int begin_aantal_geleverde_vaccins = getAantalGeleverdeVaccins(vaccin->type);
    aantal_geleverde_vaccins[vaccin->type] += vaccins_in_levering;
    ENSURE(begin_aantal_geleverde_vaccins + vaccins_in_levering == getAantalGeleverdeVaccins(vaccin->type),
           "De vaccins zijn niet succesvol geleverd!");
}

bool VaccinatieCentrum::isIedereenGevaccineerd() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling isIedereenGevaccineerd");
    int totaalAantalVaccinaties = 0;
    map<string, int>::const_iterator it;
    for (it = aantal_vaccinaties.begin(); it != aantal_vaccinaties.end(); it++) {
        totaalAantalVaccinaties += aantal_vaccinaties.at(it->first);
    }
    return totaalAantalVaccinaties == kaantal_inwoners;
}

int VaccinatieCentrum::getAantalTweedePrikken(const string &vaccin, int dag) const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getAantalTweedePrikken");
    if ((int) aantal_eerste_prikken.size() <= dag) return 0;
    MapSICIterator aantal = aantal_eerste_prikken[dag].find(vaccin);
    if (aantal == aantal_eerste_prikken[dag].end()) return 0;
    ENSURE(aantal->second >= 0, "We kunnen niet een negatief aantal 2de prikken hebben!");
    return aantal->second;
}

int VaccinatieCentrum::getAantalNietVaccinaties() const {
    REQUIRE(this->isProperlyInitialized(), "Object wasn't initialized when calling getAantalNietVaccinaties()");
    ENSURE(aantal_niet_vaccinaties >= 0, "We kunnen niet een negatief aantal niet vaccinaties hebben!");
    return aantal_niet_vaccinaties;
}



