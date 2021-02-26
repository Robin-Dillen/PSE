//
// Created by nibor on 26/02/2021.
//

#include "VaccinatieSimulatie.h"

VaccinatieSimulatie::VaccinatieSimulatie(Hub *fhub) : fhub(fhub) {}

void VaccinatieSimulatie::start() {
    unsigned int date = 0; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!fhub->isIedereenGevaccineerd()) {

        if (date == fhub->getLeveringenInterval()) {
            fhub->ontvangLevering(fhub->getKaantalVaccinsPerLevering());
            // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
        }

        // stuur signaal nieuwe dag
        fhub->nieuweDag();

        // increase date
        date = (date + 1) % 8;
    }
}
