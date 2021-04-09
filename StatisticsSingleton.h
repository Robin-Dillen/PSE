
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
    static StatisticsSingleton &getInstance() {
        static StatisticsSingleton instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    //https://stackoverflow.com/a/46432381/10875953
    typedef map<const VaccinatieCentrum *const, map<string, StatisticsSingletonData> > container_type;
    typedef container_type::iterator iterator;
    typedef container_type::const_iterator const_iterator;

    inline iterator begin() { return data.begin(); }

    inline iterator end() { return data.end(); }

    void addGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type, int aantal);

    int getGeleverdeVaccins(const VaccinatieCentrum *const centrum, const string &type) const;

    void addVaccinatie(const VaccinatieCentrum *const centrum, const string &type, int aantal);

    int getAantalVaccinaties(const VaccinatieCentrum *const centrum, const string &type) const;

private:
    StatisticsSingleton() {

    }                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    StatisticsSingleton(StatisticsSingleton const &);              // Don't Implement
    void operator=(StatisticsSingleton const &); // Don't implement

    map<const VaccinatieCentrum *const, map<string, StatisticsSingletonData> > data;
};


#endif //PSE_STATISTICSSINGLETON_H
