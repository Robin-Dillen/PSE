
#include "StatisticsSingleton.h"
#include "lib/DesignByContract.h"

void StatisticsSingleton::addGeleverdeVaccins(const string &type, int aantal) {
    if (aantal_geleverde_vaccins.find(type) == aantal_geleverde_vaccins.end()) {
        aantal_geleverde_vaccins[type] = aantal;
        return;
    }
    aantal_geleverde_vaccins[type] += aantal;
}

void StatisticsSingleton::addVaccinatie(const string &centrum, const string &type, int aantal) {
    if (aantal_vaccinaties.find(centrum) == aantal_vaccinaties.end()) {
        aantal_vaccinaties[centrum][type] = aantal;
        return;
    }
    if (aantal_vaccinaties[centrum].find(type) == aantal_vaccinaties[centrum].end()) {
        aantal_vaccinaties[centrum][type] = aantal;
        return;
    }
    aantal_vaccinaties[centrum][type] += aantal;
}

int StatisticsSingleton::getGeleverdeVaccins(const string &type) const {
    map<string, int>::const_iterator aantal = aantal_geleverde_vaccins.find(type);
    if (aantal == aantal_geleverde_vaccins.end()) return 0;
    return aantal->second;
}

int StatisticsSingleton::getAantalVaccinaties(const string &centrum, const string &type) const {
    map<string, map<string, int> >::const_iterator c = aantal_vaccinaties.find(centrum);
    if (c == aantal_vaccinaties.end()) return 0;
    map<string, int>::const_iterator aantal = aantal_vaccinaties.at(centrum).find(type);
    if (aantal == aantal_vaccinaties.at(centrum).end()) return 0;
    return aantal->second;

}
