#ifndef PSE_HUB_H
#define PSE_HUB_H

#include <map>
#include <string>
#include <vector>
#include <cmath> // voor ceil
#include <iostream> // for testing

#include "VaccinatieCentrum.h"
#include "lib/DesignByContract.h"

using namespace std;

class Hub {
public:

    /**
    \n ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    */
    Hub(const unsigned int kaantal_vaccins_per_levering, const unsigned int kleveringen_interval, const unsigned int kaantal_vaccins_per_lading);

    /**
     * @return unsigned int :geeft terug om de hoeveel dagen een levering vaccins binnenkomt
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
     */
    unsigned int getLeveringenInterval() const;

    /**
     * update de verbonden centra
     * @param fverbondenCentra: een map met als key een string(naam van vaccinatie centrum) en als value een pointer naar een vaccinatie centrum
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setFverbondenCentra");
     */
    void setFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra);

    /**
     * update de verbonden centra
     * @param fverbondenCentra: een vector met een aantal verbonden centra
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setFverbondenCentra");
     */
    void setFverbondenCentra(const vector<VaccinatieCentrum *> &fverbondenCentra);

    /**
     * update het aantal vaccins
     * @param aantalVaccins: unsigned int
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccins");
     */
    void setAantalVaccins(unsigned int aantalVaccins);

    /**
     * update het aantal vaccins dat geleverd wordt bij elke levering
     * @param aantalVaccinsPerLevering: unsigned int
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinsPerLevering");
     */
    void setAantalVaccinsPerLevering(unsigned int aantalVaccinsPerLevering);

    /**
     * update de tijdsduur tussen elke levering van vaccins(in dagen)
     * @param leveringenInterval: unsigned int
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setLeveringenInterval");
     */
    void setLeveringenInterval(unsigned int leveringenInterval);

    /**
     * update het aantal vaccins dat geleverd kan worden aan een vaccinatie centrum
     * @param aantalVaccinsPerLading: unsigned int
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling setAantalVaccinsPerLading");
     */
    void setAantalVaccinsPerLading(unsigned int aantalVaccinsPerLading);

    /*!
     * @return geeft het totaal aantal vaccinaties terug: som van de vaccinaties van de vaccinatie centra
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling totaalAantalVaccinaties");
     */
    unsigned int getTotaalAantalVaccinaties() const; // wordt wss niet gebruikt
    const unsigned int getKaantalVaccinsPerLevering() const;

    /*!
     * @return geeft true terug als iedereen gevaccineerd is, anders geeft de functie false terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
     */
    bool isIedereenGevaccineerd() const;

    /*!
     * #TODO
     */
    void nieuweDag();

    void ontvangLevering(unsigned int aantal_vaccins);

private:

    //functions
    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    void verdeelVaccins();

    // const attributes
    const unsigned int kaantal_vaccins_per_levering;
    const unsigned int kleveringen_interval;
    const unsigned int kaantal_vaccins_per_lading;

    const Hub *_initCheck; // pointer naar zichzelf om te checken of het object correct geïnitialseert is

    // changing attributes
    map<string, VaccinatieCentrum *> fverbonden_centra; // slaagt alle vaccinatie centra op met zoeksleutel: name

    unsigned int aantal_vaccins; // aantal vaccins in de hub
};


#endif //PSE_HUB_H
