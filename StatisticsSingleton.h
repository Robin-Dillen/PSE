
#ifndef PSE_STATISTICSSINGLETON_H
#define PSE_STATISTICSSINGLETON_H

#include <map>
#include <string>

using namespace std;

class Vaccin;

class VaccinatieCentrum;

//https://stackoverflow.com/a/1008289/10875953
class StatisticsSingleton {
public:
    static StatisticsSingleton &getInstance() {
        static StatisticsSingleton instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    void addGeleverdeVaccins(const string &type, int aantal);

    int getGeleverdeVaccins(const string &type) const;

    void addVaccinatie(const string &centrum, const string &type, int aantal);

    int getAantalVaccinaties(const string &centrum, const string &type) const;

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

    map<string, int> aantal_geleverde_vaccins;
    map<string, map<string, int> > aantal_vaccinaties;
};


#endif //PSE_STATISTICSSINGLETON_H
