
#include "StatisticsSingleton.h"
#include "lib/DesignByContract.h"

#include <algorithm>


StatisticsSingletonData::StatisticsSingletonData(int aantalVaccinaties, int aantalGeleverdeVaccins)
        : aantal_vaccinaties(aantalVaccinaties), aantal_geleverde_vaccins(aantalGeleverdeVaccins) {}

StatisticsSingletonData::StatisticsSingletonData() : aantal_vaccinaties(0), aantal_geleverde_vaccins(0) {}

void StatisticsSingleton::addGeleverdeVaccins(const map<string, int> &geleverd) {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    for (map<string, int>::const_iterator levering = geleverd.begin(); levering != geleverd.end(); ++levering) {
        if (data.find(levering->first) == data.end()) {
            data[levering->first].aantal_geleverde_vaccins = levering->second;
            data[levering->first].aantal_vaccinaties = 0;
            data[levering->first].aantal_eerste_prikken = 0;
        } else {
            data[levering->first].aantal_geleverde_vaccins += levering->second;
        }
        ENSURE(data[levering->first].aantal_geleverde_vaccins == getGeleverdeVaccins()[levering->first],
               "Het setten van de map was nie succesvol!");
    }

}

void StatisticsSingleton::setEerstePrikken(const map<string, int> &eerste_prikken) {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    int begin_totaal = getTotaalEerstePrikken();
    for (map<string, int>::const_iterator eerste_prik = eerste_prikken.begin();
         eerste_prik != eerste_prikken.end(); ++eerste_prik) {
        if (data.find(eerste_prik->first) == data.end()) {
            data[eerste_prik->first].aantal_geleverde_vaccins = 0;
            data[eerste_prik->first].aantal_vaccinaties = 0;
            data[eerste_prik->first].aantal_eerste_prikken = eerste_prik->second;
        } else {
            begin_totaal -= data[eerste_prik->first].aantal_eerste_prikken;
            data[eerste_prik->first].aantal_eerste_prikken = eerste_prik->second;
        }
        begin_totaal += eerste_prik->second;
        ENSURE(begin_totaal == getTotaalEerstePrikken(), "De eerste prikken zijn niet succesvol geset!");
    }
}

void StatisticsSingleton::setAantalVaccinaties(const map<string, int> &aantal) {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    int begin_totaal = getTotaalVolledigeVaccinaties();
    for (map<string, int>::const_iterator aantal_vaccins = aantal.begin();
         aantal_vaccins != aantal.end(); ++aantal_vaccins) {
        if (data.find(aantal_vaccins->first) == data.end()) {
            data[aantal_vaccins->first].aantal_geleverde_vaccins = 0;
            data[aantal_vaccins->first].aantal_eerste_prikken = 0;
            data[aantal_vaccins->first].aantal_vaccinaties = aantal_vaccins->second;
        } else {
            begin_totaal -= data[aantal_vaccins->first].aantal_vaccinaties;
            data[aantal_vaccins->first].aantal_vaccinaties = aantal_vaccins->second;
        }
        begin_totaal += data[aantal_vaccins->first].aantal_vaccinaties;
        ENSURE(begin_totaal == getTotaalVolledigeVaccinaties(),
               "Het setten van de volledige vaccinaties is nie succesvol!");
    }
}

void StatisticsSingleton::setTotaalAantalMensen(int totaalAantalMensen) {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    totaal_aantal_mensen = totaalAantalMensen;
    ENSURE(getTotaalAantalMensen() == totaalAantalMensen, "Het totaal aantal mensen is niet succesvol geset!");
}

int StatisticsSingleton::getTotaalAantalMensen() const {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    return totaal_aantal_mensen;
}

int StatisticsSingleton::getTotaalEerstePrikken() const {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    int totaal = 0;
    for (map<std::string, StatisticsSingletonData>::const_iterator d = data.begin(); d != data.end(); d++) {
        totaal += d->second.aantal_eerste_prikken;
    }
    ENSURE(totaal >= 0, "Het totaal aantal eerste prikken kan niet negatief zijn!");
    return totaal;
}

int StatisticsSingleton::getTotaalVolledigeVaccinaties() const {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    int totaal = 0;
    for (map<std::string, StatisticsSingletonData>::const_iterator d = data.begin(); d != data.end(); d++) {
        totaal += d->second.aantal_vaccinaties;
    }
    ENSURE(totaal >= 0, "Het totaal aantal volledige vaccinaties kan niet negatief zijn!");
    return totaal;
}

map<string, int> StatisticsSingleton::getGeleverdeVaccins() const {
    REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
    map<string, int> geleverde_vaccins;
    for (map<std::string, StatisticsSingletonData>::const_iterator d = data.begin(); d != data.end(); d++) {
        geleverde_vaccins[d->first] = d->second.aantal_geleverde_vaccins;
    }
    return geleverde_vaccins;
}

bool StatisticsSingleton::isProperlyInitialized() const {
    return this == _initCheck;
}

