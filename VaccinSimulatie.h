//
// Created by Niels on 2/06/2021.
//

#ifndef PSE_VACCINSIMULATIE_H
#define PSE_VACCINSIMULATIE_H

#include "Parser.h"
#include "VaccinatieCentrum.h"
#include "Output.h"
#include "Hub.h"
#include "Utils.h"
#include "StatisticsSingleton.h"
#include "Vaccins.h"
#include "Lib.h"

#include <QApplication>
#include <QTime>

class MainWindow;

class VaccinSimulatie {
public:
    VaccinSimulatie(vector<Hub *> &hubs, std::vector<VaccinatieCentrum *> &vaccinatie_centra, const string &testfilename,
    bool c_out = true);
    void setWindow(MainWindow* w);
    void start();
    void nextDay();
    void PreviousDay();

private:
    /*!
     * pauzes the simulation for a given time length
     * @param time how long the simulation is pauzes (in micro secs)
     */
    void delay(int time);

    MainWindow* window;
    int day = 0;
    vector<Hub*> hubs;
    vector<VaccinatieCentrum*> vaccinatieCentra;
    string filename1;
    string filename2;

};


#endif //PSE_VACCINSIMULATIE_H
