
#ifndef PSE_VACCINS_H
#define PSE_VACCINS_H

#include <string>
#include <vector>
#include <deque>
#include <map>
#include "lib/DesignByContract.h"

using namespace std;

struct Vaccin {
    Vaccin(const string &type, const int levering, const int interval, const int transport, const int hernieuwing,
           const int temperatuur) : type(type), levering(levering), interval(interval), transport(transport),
                                    hernieuwing(hernieuwing), temperatuur(temperatuur),
                                    tijd_tot_nieuwe_levering(interval) {}

    Vaccin() : levering(0), interval(0), transport(0), hernieuwing(0), temperatuur(0),
               tijd_tot_nieuwe_levering(interval) {}

    const string type;
    const int levering; // geleverd aan hub
    const int interval;
    const int transport; // geleverd aan VaccinatieCentrum
    const int hernieuwing;
    const int temperatuur;

    int tijd_tot_nieuwe_levering;
    int aantal;
    map<string, deque<int> > gereserveerd;
    map<string, deque<int> > extra_gereserveerd;
};


class VaccinsFactorySingleton {
public:
    virtual ~VaccinsFactorySingleton() {
        for (vector<Vaccin *>::iterator v = vaccins.begin(); v != vaccins.end(); v++) {
            delete *v;
        }
    }

    /*!
     * geeft de instance van de factory terug
     * @return VaccinsFactorySingleton&
     */
    static VaccinsFactorySingleton &getInstance() {
        static VaccinsFactorySingleton instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    /*!
     * maakt een vaccin aan en geeft een pointer van dit nieuwe vaccin terug
     * @param type naam van het vaccin
     * @param levering hoe veel vaccins aan hub geleverd wordt
     * @param interval hoe vaak geleverd wordt aan Hub
     * @param transport per hoeveel vaccins er naar VaccinatieCentrum gestuurd wordt
     * @param hernieuwing hoeveel tijd er zit tussen de eerste en de 2de prik
     * @param temperatuur bij welke temperatuur de vaccins bijgehouden moeten worden
     * @return pointer naar het nieuwe vaccin
     * \n REQUIRE(this == _initCheck, "Object wasn't initialized when calling getInstance");
     * \n REQUIRE(!type.empty(), "Type name cannot be empty!");
     * \n REQUIRE(levering > 0, "levering moet groter zijn dan 0!");
     * \n REQUIRE(interval > 0, "Interval moet groter zijn dan 0!");
     * \n REQUIRE(transport > 0, "Transport moet groter zijn dan 0!");
     * \n REQUIRE(hernieuwing >= 0, "Hernieuwing mag niet negatief zijn!");
     */
    Vaccin *
    getVaccin(const string &type, const int levering, const int interval, const int transport, const int hernieuwing,
              const int temperatuur) {
        REQUIRE(this == _initCheck, "Object wasn't initialized when calling getInstance");
        REQUIRE(!type.empty(), "Type name cannot be empty!");
        REQUIRE(levering > 0, "levering moet groter zijn dan 0!");
        REQUIRE(interval > 0, "Interval moet groter zijn dan 0!");
        REQUIRE(transport > 0, "Transport moet groter zijn dan 0!");
        REQUIRE(hernieuwing >= 0, "Hernieuwing mag niet negatief zijn!");

        Vaccin *V = new Vaccin(type, levering, interval, transport, hernieuwing, temperatuur);
        vaccins.push_back(V);
        return V;
    }

    /**
  * @return geeft terug of het object correct is geïnitialiseert
  */
    bool isProperlyInitialized() const {
        return this == _initCheck;
    }

private:
    VaccinsFactorySingleton() : _initCheck(
            this) {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    VaccinsFactorySingleton(VaccinsFactorySingleton const &); // Don't Implement
    void operator=(VaccinsFactorySingleton const &); // Don't implement

    vector<Vaccin *> vaccins;
    VaccinsFactorySingleton *const _initCheck;
};


#endif //PSE_VACCINS_H
