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
#include "Output.h"

using namespace std;

int main(int argc, char const *argv[]) {
    vector<string> args = vector<string>(argv + 1, argv + argc);
    Parser P(args[0]);
    Hub *H = P.getFhub();

    // clear the output file
    std::ofstream ofs;
    ofs.open("../Output.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    // start simulatie

    unsigned int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    unsigned int current_day = 1; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!H->isIedereenGevaccineerd() && (!end_day || current_day < end_day)) {

        if (current_day % H->getLeveringenInterval() == 0) {
            // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
            H->ontvangLevering(H->getKaantalVaccinsPerLevering());
        }

        // stuur signaal nieuwe dag
        H->nieuweDag();

        // output
        Output::makeOutputFile(H, current_day);

        // increase current_day
        current_day++;
    }

    unsigned int years = current_day / 356;
    current_day -= years * 356;
    unsigned int months = current_day / 30;
    current_day -= months * 30;
    unsigned int weeks = current_day / 7;
    current_day -= weeks * 7;

    cout << "de simulatie duurde " << years << " jaren, " << months << string(" ") + (months == 1 ? "maand, " : "maanden, ") << weeks
         << string(" ") + (weeks == 1 ? "week" : "weken") + " en " << current_day
         << string(" ") + +(current_day == 1 ? "dag" : "dagen") + +"." << endl;
    return 0;
}

