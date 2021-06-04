
#include "StatisticsSingleton.h"
#include "lib/DesignByContract.h"


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

void StatisticsSingleton::setAantalVaccinaties(const map<string, int> &aantal) {
    for (map<string, int>::const_iterator levering = aantal.begin(); levering != aantal.end(); ++levering) {
        if (data.find(levering->first) == data.end()) {
            data[levering->first].aantal_geleverde_vaccins = 0;
            data[levering->first].aantal_eerste_prikken = 0;
        }
        data[levering->first].aantal_vaccinaties = levering->second;
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
