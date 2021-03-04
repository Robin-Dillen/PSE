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
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra");
     */
    void addFverbondenCentra(const map<string, VaccinatieCentrum *> &fverbondenCentra);

    /**
     * voegt centra toe aan de verbonden centra
     * @param fverbondenCentra: een vector met een aantal verbonden centra
     * @return void
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling addFverbondenCentra");
     * \n ENSURE(fverbonden_centra.size() == fverbondenCentra.size() + start_size, "De centra zijn niet (volledig) Toegevoegd");
     */
    void addFverbondenCentra(const vector<VaccinatieCentrum *> &fverbondenCentra);

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

    /*!
     * @return geeft het aantal vaccins per levering terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getKaantalVaccinsPerLevering");
     */
    const unsigned int getKaantalVaccinsPerLevering() const;

    /*!
     * @return geeft true terug als iedereen gevaccineerd is, anders geeft de functie false terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling isIedereenGevaccineerd");
     */
    bool isIedereenGevaccineerd() const;

    /*!
     * start een nieuwe dag, verdeelt de vaccins en stuurt een signaal naar de vaccinatiecentra
     * REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling nieuweDag");
     */
    void nieuweDag();

    /*!
     * ontvangt een levering, voegt het aantal geleverde vaccins toe aan het aantal vaccins van de hub.
     * @param aantal_vaccins het aanta geleverde vaccins
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling ontvangLevering");
     */
    void ontvangLevering(unsigned int aantal_vaccins);

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

    /*!
     * verdeelt de vaccins over alle verbonden vaccinatie centra
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling verdeelVaccins");
     * \n ENSURE(aantal vaccins >= 0, "You can't have less than 0 vaccins!");
     */
    void verdeelVaccins();

    /*!
     * geeft het minimum aantal leveringen terug dat nodig is om de capaciteit van het gegeven vaccinaticentrum te bereiken
     * @param it iterator
     * @return het minimum aantal leveringen
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling minAantalLeveringen");
     */
    unsigned int minAantalLeveringen(const map<string, VaccinatieCentrum *>::const_iterator &it) const;

    /*!
     * @return geeft de verbonden centra terug
     * \n REQUIRE(this->isProperlyInitialized(), "Parser wasn't initialized when calling getFverbondenCentra");
     */
    const map<string, VaccinatieCentrum *> &getFverbondenCentra() const;

private:

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
