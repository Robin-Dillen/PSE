
#ifndef PSE_STATISTICSSINGLETON_H
#define PSE_STATISTICSSINGLETON_H

#include <map>
#include <deque>
#include <string>
#include <QObject>

using namespace std;

class Vaccin;
class VaccinatieCentrum;

struct StatisticsSingletonData {
    StatisticsSingletonData(int aantalVaccinaties, int aantalGeleverdeVaccins);
    StatisticsSingletonData();

    int aantal_eerste_prikken;
    int aantal_vaccinaties;
    int aantal_geleverde_vaccins;
};

//https://stackoverflow.com/a/1008289/10875953
class StatisticsSingleton : public QObject {
Q_OBJECT
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

    void addGeleverdeVaccins(const map<string, int> &geleverd);

    void setEerstePrikken(const map<string, int> &eerste_prikken);

    void setAantalVaccinaties(const map<string, int> &aantal);

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    void setTotaalAantalMensen(int totaalAantalMensen);

    int getTotaalEerstePrikken() const;

    int getTotaalVolledigeVaccinaties() const;

    map<string, int> getGeleverdeVaccins() const;

signals:

    void dataChange();

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

    map<std::string, StatisticsSingletonData> data;
    int totaal_aantal_mensen;
    const StatisticsSingleton *const _initCheck;
public:
    int getTotaalAantalMensen() const;
};


#endif //PSE_STATISTICSSINGLETON_H
