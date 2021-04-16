
#ifndef PSE_STATISTICSSINGLETON_H
#define PSE_STATISTICSSINGLETON_H

#include <map>
#include <string>

using namespace std;

class Vaccin;
class VaccinatieCentrum;

struct StatisticsSingletonData {
    StatisticsSingletonData(int aantalVaccinaties, int aantalGeleverdeVaccins);
    StatisticsSingletonData();

    int aantal_vaccinaties;
    int aantal_geleverde_vaccins;
};

//https://stackoverflow.com/a/1008289/10875953
class StatisticsSingleton {
public:
    /*!
     * geeft de enige instance van StatisticsSingleton terug
     * @return reference naar de enige instance van het object
     */
    static StatisticsSingleton &getInstance() {
        static StatisticsSingleton instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    //https://stackoverflow.com/a/46432381/10875953
    typedef map<const VaccinatieCentrum *const, map<string, StatisticsSingletonData> > container_type;
    typedef container_type::iterator iterator;
    typedef container_type::const_iterator const_iterator;

    /*!
     * geeft een (begin) iterator terug
     * @return iterator
     */
    inline iterator begin() { return data.begin(); }

    /*!
     * geeft een (end) iterator terug
     * @return iterator
     */
    inline iterator end() { return data.end(); }

    /*!
     * voegt vaccins van een type aan een centrum toe aan het totaal aantal geleverde vaccins
     * @param centrum VaccinatieCentrum
     * @param type naam van het vaccin
     * @param aantal aantal vaccins dat toegevoegt wordt
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addGeleverdeVaccins");
     * \n REQUIRE(aantal >= 0, "Het aantal mag niet negatief zijn!");
     */
    void addGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type, int aantal);

    /*!
     * geeft het totaal aantal geleverde vaccins terug van een centrum, en een bepaald vaccin
     * @param centrum VaccinatieCentrum
     * @param type van het vaccins
     * @return int
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling getGeleverdeVaccins");
     * \n REQUIRE(aantal >= 0, "Het aantal mag niet negatief zijn!");
     */
    int getGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type) const;

    /*!
     * voegt vaccinaties van een type aan een centrum toe aan het totaal aantal vaccinaties
     * @param centrum VaccinatieCentrum
     * @param type van het vaccins
     * @param aantal aantal vaccinaties dat toegevoegt wordt
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addVaccinatie");
     * \n ENSURE(aantal->second.aantal_geleverde_vaccins >= 0, "het aantal geleverde vaccins kan niet negatief zijn!");
     */
    void addVaccinatie(const VaccinatieCentrum *const centrum, const string &type, int aantal);

    /*!
     * geeft het totaal aantal vaccinaties terug van een centrul en een bepaald vaccin
     * @param centrum VaccinatieCentrum
     * @param type naam van het vaccin
     * @return int
     * \n REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling getAantalVaccinaties");
     * \n ENSURE(aantal->second.aantal_vaccinaties >= 0, "het aantal vaccinaties kan niet negatief zijn!");
     */
    int getAantalVaccinaties(const VaccinatieCentrum *const centrum, const string &type) const;

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

private:
    StatisticsSingleton() : _initCheck(this) {

    }                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    StatisticsSingleton(StatisticsSingleton const &);              // Don't Implement
    void operator=(StatisticsSingleton const &); // Don't implement

    map<const VaccinatieCentrum *const, map<string, StatisticsSingletonData> > data;
    const StatisticsSingleton *const _initCheck;
};


#endif //PSE_STATISTICSSINGLETON_H
