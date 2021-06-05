
#include "StatisticsSingleton.h"
#include "lib/DesignByContract.h"

#include <algorithm>


StatisticsSingletonData::StatisticsSingletonData(int aantalVaccinaties, int aantalGeleverdeVaccins)
        : aantal_vaccinaties(aantalVaccinaties), aantal_geleverde_vaccins(aantalGeleverdeVaccins) {}

StatisticsSingletonData::StatisticsSingletonData() : aantal_vaccinaties(0), aantal_geleverde_vaccins(0) {}

void StatisticsSingleton::addGeleverdeVaccins(const map<string, int> &geleverd) {
    for (map<string, int>::const_iterator levering = geleverd.begin(); levering != geleverd.end(); ++levering) {
        if (data.find(levering->first) == data.end()) {
            data[levering->first].aantal_geleverde_vaccins = levering->second;
            data[levering->first].aantal_vaccinaties = 0;
            data[levering->first].aantal_eerste_prikken = 0;
        } else {
            data[levering->first].aantal_geleverde_vaccins += levering->second;
        }
    }
}

void StatisticsSingleton::setEerstePrikken(const map<string, int> &eerste_prikken) {
    for (map<string, int>::const_iterator eerste_prik = eerste_prikken.begin();
         eerste_prik != eerste_prikken.end(); ++eerste_prik) {
        if (data.find(eerste_prik->first) == data.end()) {
            data[eerste_prik->first].aantal_geleverde_vaccins = 0;
            data[eerste_prik->first].aantal_vaccinaties = 0;
            data[eerste_prik->first].aantal_eerste_prikken = eerste_prik->second;
        } else {
            data[eerste_prik->first].aantal_eerste_prikken = eerste_prik->second;
        }
    }
}

void StatisticsSingleton::setAantalVaccinaties(const map<string, int> &aantal) {
    for (map<string, int>::const_iterator aantal_vaccins = aantal.begin();
         aantal_vaccins != aantal.end(); ++aantal_vaccins) {
        if (data.find(aantal_vaccins->first) == data.end()) {
            data[aantal_vaccins->first].aantal_geleverde_vaccins = 0;
            data[aantal_vaccins->first].aantal_eerste_prikken = 0;
        }
        data[aantal_vaccins->first].aantal_vaccinaties = aantal_vaccins->second;
    }
}

void StatisticsSingleton::setTotaalAantalMensen(int totaalAantalMensen) {
    totaal_aantal_mensen = totaalAantalMensen;
}

int StatisticsSingleton::getTotaalAantalMensen() const {
    return totaal_aantal_mensen;
}

int StatisticsSingleton::getTotaalEerstePrikken() const {
    int totaal = 0;
    for (map<std::string, StatisticsSingletonData>::const_iterator d = data.begin(); d != data.end(); d++) {
        totaal += d->second.aantal_eerste_prikken;
    }
    return totaal;
}

int StatisticsSingleton::getTotaalVolledigeVaccinaties() const {
    int totaal = 0;
    for (map<std::string, StatisticsSingletonData>::const_iterator d = data.begin(); d != data.end(); d++) {
        totaal += d->second.aantal_vaccinaties;
    }
    return totaal;
}

map<string, int> StatisticsSingleton::getGeleverdeVaccins() const {
    map<string, int> geleverde_vaccins;
    for (map<std::string, StatisticsSingletonData>::const_iterator d = data.begin(); d != data.end(); d++) {
        geleverde_vaccins[d->first] = d->second.aantal_geleverde_vaccins;
    }
    return geleverde_vaccins;
}

