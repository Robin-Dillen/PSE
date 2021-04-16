
#include "StatisticsSingleton.h"
#include "lib/DesignByContract.h"

void StatisticsSingleton::addGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type, int aantal) {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addGeleverdeVaccins");
    REQUIRE(aantal >= 0, "Het aantal mag niet negatief zijn!");
    data[centrum][type].aantal_geleverde_vaccins += aantal;
}

void
StatisticsSingleton::addVaccinatie(const VaccinatieCentrum *const centrum, const string &type, int aantal) {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addVaccinatie");
    REQUIRE(aantal >= 0, "Het aantal mag niet negatief zijn!");
    data[centrum][type].aantal_vaccinaties += aantal;
}

int StatisticsSingleton::getGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type) const {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling getGeleverdeVaccins");
    const_iterator c = data.find(centrum);
    if (c == data.end()) return 0;
    map<string, StatisticsSingletonData>::const_iterator aantal = data.at(centrum).find(type);
    if (aantal == data.at(centrum).end()) return 0;
    ENSURE(aantal->second.aantal_geleverde_vaccins >= 0, "het aantal geleverde vaccins kan niet negatief zijn!");
    return aantal->second.aantal_geleverde_vaccins;
}

int StatisticsSingleton::getAantalVaccinaties(const VaccinatieCentrum *const centrum, const string &type) const {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling getAantalVaccinaties");
    const_iterator c = data.find(centrum);
    if (c == data.end()) return 0;
    map<string, StatisticsSingletonData>::const_iterator aantal = data.at(centrum).find(type);
    if (aantal == data.at(centrum).end()) return 0;
    ENSURE(aantal->second.aantal_vaccinaties >= 0, "het aantal vaccinaties kan niet negatief zijn!");
    return aantal->second.aantal_vaccinaties;
}

bool StatisticsSingleton::isProperlyInitialized() const {
    return _initCheck == this;
}

StatisticsSingletonData::StatisticsSingletonData(int aantalVaccinaties, int aantalGeleverdeVaccins)
        : aantal_vaccinaties(aantalVaccinaties), aantal_geleverde_vaccins(aantalGeleverdeVaccins) {}

StatisticsSingletonData::StatisticsSingletonData() : aantal_vaccinaties(0), aantal_geleverde_vaccins(0) {}

