#include <iostream>
#include <vector>

#include "Parser.h"
#include "Output.h"
#include "Hub.h"
#include "Utils.h"

using namespace std;

inline void Simulatie(const string &testfilename, bool c_out = true) {
    Parser P(testfilename);
    vector<Hub *> hubs = P.getFhubs();

    size_t pos = testfilename.find("test");
    ENSURE(pos != string::npos, "Given argument doesn't include a test file!");
    string filename = "Output_" + testfilename.substr(pos, 7);

    // clear/create the output file
    Output::makeOutputFile(filename);

    // start simulatie

    int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    int current_day = 0; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    bool break_ = false;
    for (unsigned int i = 0; i < hubs.size(); i++) {
        // output
        Output::addToOutputFile(hubs[i], current_day, filename);
    }
    while ((!end_day || current_day < end_day) && !break_) {
        current_day++;
        for (unsigned int i = 0; i < hubs.size(); i++) {
            if (hubs[i]->isIedereenGevaccineerd()) {
                break_ = true;
                continue;
            }
            break_ = false;
            // increase current_day

            if (current_day % hubs[i]->getLeveringenInterval() == 0) {
                // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
                // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
                hubs[i]->ontvangLevering(hubs[i]->getKaantalVaccinsPerLevering());
            }

            // stuur signaal nieuwe dag
            hubs[i]->nieuweDag();

            // output
            Output::addToOutputFile(hubs[i], current_day, filename);
        }
    }

    int years = current_day / 356;
    current_day -= years * 356;
    int months = current_day / 30;
    current_day -= months * 30;
    int weeks = current_day / 7;
    current_day -= weeks * 7;

    if (c_out) {
        cout << "de simulatie duurde " << years << " jaren, " << months
             << string(" ") + (months == 1 ? "maand, " : "maanden, ") << weeks
             << string(" ") + (weeks == 1 ? "week" : "weken") + " en " << current_day
             << string(" ") + +(current_day == 1 ? "dag" : "dagen") + +"." << endl;
    }
}
