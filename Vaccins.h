
#ifndef PSE_VACCINS_H
#define PSE_VACCINS_H

#include <string>
#include <map>

using namespace std;

struct Vaccin {
    Vaccin(const string &type, const int levering, const int interval, const int transport, const int hernieuwing,
           const int temperatuur) : type(type), levering(levering), interval(interval), transport(transport),
                                    hernieuwing(hernieuwing), temperatuur(temperatuur),
                                    tijd_tot_nieuwe_levering(interval) {}

    const string type;
    const int levering; // geleverd aan hub
    const int interval;
    const int transport; // geleverd aan VaccinatieCentrum
    const int hernieuwing;
    const int temperatuur;
    int tijd_tot_nieuwe_levering;
};


class VaccinsSingletonFactory {
public:
    virtual ~VaccinsSingletonFactory() {
        for (map<string, Vaccin *>::iterator v = types.begin(); v != types.end(); v++) {
            delete v->second;
        }
    }

    static VaccinsSingletonFactory &getInstance() {
        static VaccinsSingletonFactory instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Vaccin *
    getVaccin(const string &type, const int levering, const int interval, const int transport, const int hernieuwing,
              const int temperatuur) {
        if (types.find(type) != types.end()) {
            return types.at(type);
        }
        Vaccin *V = new Vaccin(type, levering, interval, transport, hernieuwing, temperatuur);
        types[type] = V;
        return V;
    }

private:
    VaccinsSingletonFactory() {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    VaccinsSingletonFactory(VaccinsSingletonFactory const &); // Don't Implement
    void operator=(VaccinsSingletonFactory const &); // Don't implement

    map<string, Vaccin *> types;
};


#endif //PSE_VACCINS_H
