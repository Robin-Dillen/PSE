
#ifndef PSE_VACCINS_H
#define PSE_VACCINS_H

#include <string>
#include <vector>

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


class VaccinsFactorySingleton {
public:
    virtual ~VaccinsFactorySingleton() {
        for (vector<Vaccin *>::iterator v = vaccins.begin(); v != vaccins.end(); v++) {
            delete *v;
        }
    }

    static VaccinsFactorySingleton &getInstance() {
        static VaccinsFactorySingleton instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Vaccin *
    getVaccin(const string &type, const int levering, const int interval, const int transport, const int hernieuwing,
              const int temperatuur) {
        Vaccin *V = new Vaccin(type, levering, interval, transport, hernieuwing, temperatuur);
        vaccins.push_back(V);
        return V;
    }

private:
    VaccinsFactorySingleton() {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    VaccinsFactorySingleton(VaccinsFactorySingleton const &); // Don't Implement
    void operator=(VaccinsFactorySingleton const &); // Don't implement

    vector<Vaccin *> vaccins;
};

enum EVaccinsPriority {
    hoog, zeer_hoog
};

struct VaccinsRequest {
    VaccinsRequest(const string &type, const int aantal, const EVaccinsPriority priority) : type(type), aantal(aantal),
                                                                                            priority(priority) {}

    const string type;
    const int aantal;
    const EVaccinsPriority priority;
};


#endif //PSE_VACCINS_H
