//
// Created by Niels on 2/06/2021.
//

#include "VaccinSimulatie.h"
#include "mainwindow.h"

VaccinSimulatie::VaccinSimulatie(vector<Hub *> &h, std::vector<VaccinatieCentrum *> &v, const string &testfilename,
bool c_out) {
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
}

void VaccinSimulatie::setWindow(MainWindow *w){
    window = w;
}

void VaccinSimulatie::start(){
    pause = false;
    while (!pause) {
        nextDay();
        delay(1000);
    }
}

void VaccinSimulatie::stop(){
    pause = true;
}

void VaccinSimulatie::nextDay(){
    OutputSingleton &output = OutputSingleton::getInstance();
    bool endOfSimulation = true;
    for (unsigned int i = 0; i < hubs.size(); i++) {
        if (!hubs[i]->isIedereenGevaccineerd()) {
            endOfSimulation = false;
        }
        else{
            continue;
        }

        // increase current_day

        map<string, Vaccin *> vaccins = hubs[i]->getVaccins();
        for(map<string,Vaccin*>::iterator it = vaccins.begin(); it != vaccins.end(); it++){
            if (day % (it->second->interval + 1) == 0) {
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
        for (unsigned int i = 0; i < vaccinatieCentra.size(); i++) {
            vaccinatieCentra[i]->nieuweDag();
            output.addToGIFile(vaccinatieCentra[i], filename2);
        }
        day++;
        window->changeDay(day);
    }
    else{
        window->endOfSimulation(day);
        stop();
    }
}


void VaccinSimulatie::previousDay(){
    day--;
    window->changeDay(day);
}


void VaccinSimulatie::delay(int time) {
    QTime dieTime = QTime::currentTime().addMSecs(time);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


