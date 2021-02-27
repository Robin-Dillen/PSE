//============================================================================
// Name        : main.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : ...
//============================================================================

#include <iostream>
#include <vector>

#include "Parser.h"

using namespace std;

int main(int argc, char const *argv[]) {
    vector<string> args = vector<string>(argv + 1, argv + argc);
    //Parser P(args[0]);
    Hub *H = new Hub(93000, 6, 2000);
    vector<VaccinatieCentrum *> centra;
    centra.push_back(new VaccinatieCentrum(7500, 540173, "Park Spoor Oost", "Noordersingel 40, Antwerpen"));
    centra.push_back(new VaccinatieCentrum(2000, 76935, "AED Studios", "Fabriekstraat 38, Lint"));
    H->setFverbondenCentra(centra);

    // start simulatie

    unsigned int date = 0; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!H->isIedereenGevaccineerd()) {

        if (date % H->getLeveringenInterval() == 0) {
            H->ontvangLevering(H->getKaantalVaccinsPerLevering());
            // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
        }

        // stuur signaal nieuwe dag
        H->nieuweDag();

        // increase date
        date++;
    }

    unsigned int years = date / 356;
    date -= years * 356;
    unsigned int months = date / 30;
    date -= months * 30;
    unsigned int weeks = date / 7;
    date -= weeks * 7;

    cout << "de simulatie duurde " << years << " jaren, " << months << " maanden, " << weeks
         << string(" ") + (weeks == 1 ? "week" : "weken") + " en " << date
         << string(" ") + +(date == 1 ? "dag" : "dagen") + +"." << endl;

    return 0;
}

