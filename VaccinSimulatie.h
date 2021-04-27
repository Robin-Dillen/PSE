#include <iostream>
#include <vector>

#include "Parser.h"
#include "VaccinatieCentrum.h"
#include "Output.h"
#include "Hub.h"
#include "Utils.h"
#include "StatisticsSingleton.h"
#include "Vaccins.h"

using namespace std;

inline void
Simulatie(vector<Hub *> &hubs, std::vector<VaccinatieCentrum *> &vaccinatie_centra, const string &testfilename,
          bool c_out = true) {

    size_t pos = testfilename.find("test");
    ENSURE(pos != string::npos, "Given argument doesn't include a test file!");

    string filename1;
    string filename2;
    if (testfilename.find("DeathTests") != string::npos) {
        filename1 = "DeathTests/Simpele_Uitvoer_" + testfilename.substr(pos, 7);
        filename2 = "DeathTests/Grafische_Impressie" + testfilename.substr(pos, 7);
    } else if (testfilename.find("WarningTests") != string::npos) {
        filename1 = "WarningTests/Simpele_Uitvoer_" + testfilename.substr(pos, 7);
        filename2 = "WarningTests/Grafische_Impressie" + testfilename.substr(pos, 7);
    } else {
        filename1 = "HappyDayTests/Simpele_Uitvoer_" + testfilename.substr(pos, 7);
        filename2 = "HappyDayTests/Grafische_Impressie" + testfilename.substr(pos, 7);
    }

    OutputSingleton &output = OutputSingleton::getInstance();
    // clear/create the output file
    output.makeOutputFile(filename1);

    output.makeOutputFile(filename2);

    // start simulatie

    int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    int current_day = 0; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    bool break_ = false;
    for (unsigned int i = 0; i < hubs.size(); i++) {
        // output
        output.addToOutputFile(hubs[i], i + 1, current_day, filename1);
    }
    while ((!end_day || current_day < end_day) && !break_) {
        cout << "dag " << current_day << ": " << endl;
        for (unsigned int i = 0; i < hubs.size(); i++) {
            if (hubs[i]->isIedereenGevaccineerd()) {
                break_ = true;
                continue;
            }
            break_ = false;
            // increase current_day

            map<string, Vaccin *> vaccins = hubs[i]->getVaccins();
            for(map<string,Vaccin*>::iterator it = vaccins.begin(); it != vaccins.end(); it++){
                if (current_day % (it->second->interval + 1) == 0) {
                    // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
                    // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
                    hubs[i]->ontvangLevering(it->first, it->second->levering);
                }
            }

            // stuur signaal nieuwe dag
            cout << "\thub " << i + 1 << ": " << endl;
            hubs[i]->nieuweDag();

            // output
            output.addToOutputFile(hubs[i], i + 1, current_day, filename1);
        }
        output.addDateToFile(current_day, filename2);
        for (unsigned int i = 0; i < vaccinatie_centra.size(); i++) {
            vaccinatie_centra[i]->nieuweDag();
            output.addToGIFile(vaccinatie_centra[i], filename2);
        }
        current_day++;
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
