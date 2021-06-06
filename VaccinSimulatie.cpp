//
// Created by Niels on 2/06/2021.
//
#include <iostream>

#include "VaccinSimulatie.h"
#include "Utils.h"
#include "Output.h"
#include "StatisticsSingleton.h"
#include "Vaccins.h"
#include "VaccinatieCentrum.h"
#include "Hub.h"


VaccinSimulatie::VaccinSimulatie(vector<Hub *> &h, std::vector<VaccinatieCentrum *> &v, const string &testfilename,
                                 bool c_out) : day(1) {
    size_t pos = testfilename.find("test");
    ENSURE(pos != string::npos, "Given argument doesn't include a test file!");
    hubs = h;
    vaccinatieCentra = v;

    if (testfilename.find("DeathTests") != string::npos) {
        filename1 = DEATH_OUTPUT_FILE_LOCATION;
        filename2 = DEATH_OUTPUT_FILE_LOCATION;
    } else if (testfilename.find("WarningTests") != string::npos) {
        filename1 = WARNING_OUTPUT_FILE_LOCATION;
        filename2 = WARNING_OUTPUT_FILE_LOCATION;
    } else {
        filename1 = HAPPY_DAY_OUTPUT_FILE_LOCATION;
        filename2 = HAPPY_DAY_OUTPUT_FILE_LOCATION;
    }

    filename1 += "Simpele_Uitvoer_" + testfilename.substr(pos, 7);
    filename2 += "Grafische_Impressie_" + testfilename.substr(pos, 7);

    OutputSingleton &output = OutputSingleton::getInstance();
    // clear/create the output file
    output.makeOutputFile(filename1 + ".txt");
    output.makeOutputFile(filename2 + ".txt");

    for (unsigned int i = 0; i < hubs.size(); i++) {
        // output
        output.addToOutputFile(hubs[i], i + 1, 0, filename1);
    }
    qTimer = new QTimer(this);
    qTimer->setInterval(200);
    connect(qTimer, SIGNAL(timeout()), this, SLOT(nextDay()));
}

void VaccinSimulatie::start() {
    qTimer->start();
}

void VaccinSimulatie::stop() {
    qTimer->stop();
}

bool VaccinSimulatie::nextDay() {
    StatisticsSingleton &stats = StatisticsSingleton::getInstance();
    OutputSingleton &output = OutputSingleton::getInstance();

    bool endOfSimulation = true;
    for (unsigned int i = 0; i < hubs.size(); i++) {
        if (!hubs[i]->isIedereenGevaccineerd()) {
            endOfSimulation = false;
        } else {
            continue;
        }
        // increase current_day
        map<string, Vaccin *> vaccins = hubs[i]->getVaccins();
        for(map<string,Vaccin*>::iterator it = vaccins.begin(); it != vaccins.end(); it++){
            if ((day - 1) % it->second->interval == 0) {
                // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
                // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
                hubs[i]->ontvangLevering(it->first, it->second->levering);
            }
        }

        // stuur signaal nieuwe dag
        hubs[i]->nieuweDag();

        // output
        output.addToOutputFile(hubs[i], i + 1, day, filename1);
    }
    if(!endOfSimulation){
        output.addDateToFile(day, filename2);
        map<string, int> totaal_vaccinaties;
        map<string, int> totaal_eerste_prikken;
        for (unsigned int i = 0; i < vaccinatieCentra.size(); i++) {
            stats.addGeleverdeVaccins(vaccinatieCentra[i]->getAantalGeleverdeVaccins1());
            vaccinatieCentra[i]->nieuweDag();
            const map<string, int> &vaccinaties = vaccinatieCentra[i]->getAantalVaccinaties1();
            for (map<string, int>::const_iterator aantal_vaccinaties = vaccinaties.begin();
                 aantal_vaccinaties != vaccinaties.end(); ++aantal_vaccinaties) {
                if (totaal_vaccinaties.find(aantal_vaccinaties->first) == totaal_vaccinaties.end())
                    totaal_vaccinaties[aantal_vaccinaties->first] = aantal_vaccinaties->second;
                else
                    totaal_vaccinaties[aantal_vaccinaties->first] += aantal_vaccinaties->second;
            }

            const map<string, deque<int>> &eerste_prikken = vaccinatieCentra[i]->getAantalEerstePrikken();
            for (map<string, deque<int>>::const_iterator aantal_eerste_prikken = eerste_prikken.begin();
                 aantal_eerste_prikken != eerste_prikken.end(); ++aantal_eerste_prikken) {
                int sum = std::accumulate(aantal_eerste_prikken->second.begin(), aantal_eerste_prikken->second.end(),
                                          0);
                if (totaal_eerste_prikken.find(aantal_eerste_prikken->first) == totaal_eerste_prikken.end())
                    totaal_eerste_prikken[aantal_eerste_prikken->first] = sum;
                else
                    totaal_eerste_prikken[aantal_eerste_prikken->first] += sum;
            }

            output.addToGIFile(vaccinatieCentra[i], filename2);
        }
        stats.setEerstePrikken(totaal_eerste_prikken);
        stats.setAantalVaccinaties(totaal_vaccinaties);

        day++;
        emit dayNrChanged(day);
    } else {
        emit stats.dataChange();
        emit endSimulation(day);
        stop();
        return false;
    }
    emit stats.dataChange();
    ExportSimulation("../SavedData/dag"+to_string(day)+".txt", this);
    return true;
}


//void VaccinSimulatie::previousDay() {
//    std::cout << "prev day" << std::endl;
//    day--;
//    emit dayNrChanged(day);
//}

const vector<Hub *> &VaccinSimulatie::getHubs() const {
    return hubs;
}

const vector<VaccinatieCentrum *> &VaccinSimulatie::getVaccinatieCentra() const {
    return vaccinatieCentra;
}

void VaccinSimulatie::updateSpeed(int newspeed) {
    qTimer->setInterval(newspeed);
}


