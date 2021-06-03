//============================================================================
// Name        : main.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : runs the simulation
//============================================================================

#include <iostream>
#include <vector>
#include <QApplication>

#include "mainwindow.h"
#include "VaccinSimulatie.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    vector<string> args = vector<string>(argv + 1, argv + argc);

    Parser P(args[0]);
    vector<Hub *> hubs = P.getFhubs();
    std::vector<VaccinatieCentrum *> vaccinatie_centra;

    for (unsigned int i = 0; i < hubs.size(); i++) {
        for (map<string, VaccinatieCentrum *>::const_iterator centrum = hubs[i]->getFverbondenCentra().begin();
             centrum != hubs[i]->getFverbondenCentra().end(); centrum++) {
            if (find(vaccinatie_centra.begin(), vaccinatie_centra.end(), centrum->second) == vaccinatie_centra.end()) {
                vaccinatie_centra.push_back(centrum->second);
            }
        }
    }
    VaccinSimulatie* s = new VaccinSimulatie(hubs, vaccinatie_centra, args[0]);
    MainWindow* w = new MainWindow;
    w->setSimulation(s);
    w->showNormal();
    s->setWindow(w);
    return a.exec();
}

