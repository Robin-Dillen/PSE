
#include "StatisticsSingleton.h"
#include "lib/DesignByContract.h"

void StatisticsSingleton::addGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type, int aantal) {
    data[centrum][type].aantal_geleverde_vaccins += aantal;
}

void
StatisticsSingleton::addVaccinatie(const VaccinatieCentrum *const centrum, const string &type, int aantal) {
    data[centrum][type].aantal_vaccinaties += aantal;
}

int StatisticsSingleton::getGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type) const {
    const_iterator c = data.find(centrum);
    if (c == data.end()) return 0;
    map<string, StatisticsSingletonData>::const_iterator aantal = data.at(centrum).find(type);
    if (aantal == data.at(centrum).end()) return 0;
    return aantal->second.aantal_geleverde_vaccins;
}

int StatisticsSingleton::getAantalVaccinaties(const VaccinatieCentrum *const centrum, const string &type) const {
    const_iterator c = data.find(centrum);
    if (c == data.end()) return 0;
    map<string, StatisticsSingletonData>::const_iterator aantal = data.at(centrum).find(type);
    if (aantal == data.at(centrum).end()) return 0;
    return aantal->second.aantal_vaccinaties;

}

StatisticsSingletonData::StatisticsSingletonData(int aantalVaccinaties, int aantalGeleverdeVaccins)
        : aantal_vaccinaties(aantalVaccinaties), aantal_geleverde_vaccins(aantalGeleverdeVaccins) {}

StatisticsSingletonData::StatisticsSingletonData() : aantal_vaccinaties(0), aantal_geleverde_vaccins(0) {}

