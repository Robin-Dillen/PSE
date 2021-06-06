
#ifndef PSE_STATISTICSSINGLETON_H
#define PSE_STATISTICSSINGLETON_H

#include <map>
#include <deque>
#include <string>
#include <QObject>
#include "lib/DesignByContract.h"

using namespace std;

struct Vaccin;

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

    /*!
     * voegt vaccins toe aan het totaal aantal geleverde vaccins
     * @param geleverd const map<string, int> &
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * \n ENSURE(data[levering->first].aantal_geleverde_vaccins == getGeleverdeVaccins()[levering->first], "Het setten van de map was nie succesvol!");
     */
    void addGeleverdeVaccins(const map<string, int> &geleverd);

    /*!
     * zet het totaal aantal eerste prikken
     * @param eerste_prikken const map<string, int> &
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * \n ENSURE(begin_totaal + eerste_prik->second == getTotaalEerstePrikken(), "De eerste prikken zijn niet succesvol geset!");
     */
    void setEerstePrikken(const map<string, int> &eerste_prikken);

    /*!
     * zet het aantal volledige vaccinateis
     * @param aantal const map<string, int> &
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * \n ENSURE(begin_totaal == getTotaalVolledigeVaccinaties(), "Het setten van de volledige vaccinaties is nie succesvol!");
     */
    void setAantalVaccinaties(const map<string, int> &aantal);

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     */
    bool isProperlyInitialized() const;

    /*!
     * zet het totaal aantal mensen
     * @param totaalAantalMensen int
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * \n ENSURE(getTotaalAantalMensen() == totaalAantalMensen, "Het totaal aantal mensen is niet succesvol geset!");
     */
    void setTotaalAantalMensen(int totaalAantalMensen);

    /*!
     * geeft het totaal aantal eerste prikken terug
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * @return int
     * \n ENSURE(totaal >= 0, "Het totaal aantal eerste prikken kan niet negatief zijn!");
     */
    int getTotaalEerstePrikken() const;

    /*!
     * geeft het totaal aantal volledige vaccinaties terug
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * @return int
     * \n ENSURE(totaal >= 0, "Het totaal aantal volledige vaccinaties kan niet negatief zijn!");
     */
    int getTotaalVolledigeVaccinaties() const;

    /*!
     * geeft terug hoeveel vaccins er in totaal al zijn geleverd
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * @return map<string, int
     */
    map<string, int> getGeleverdeVaccins() const;

    /*!
     * geeft het totaal aantal mensen terug
     * \n REQUIRE(isProperlyInitialized(), "The object isn't initialized when calling this function");
     * @return int
     */
    int getTotaalAantalMensen() const;

signals:

    /*!
     * stuurt een signaal dat de data is verandert
     */
    void dataChange();

private:
    StatisticsSingleton() : _initCheck(this) {
        ENSURE(isProperlyInitialized(), "The object isn't properly initialized when exiting the constructor!");
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
};


#endif //PSE_STATISTICSSINGLETON_H
