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
#include <QTimer>

class VaccinSimulatie : public QObject {
Q_OBJECT
public:
    VaccinSimulatie(vector<Hub *> &hubs, std::vector<VaccinatieCentrum *> &vaccinatie_centra,
                    const string &testfilename,
                    bool c_out = true);

    const vector<Hub *> &getHubs() const;

    const vector<VaccinatieCentrum *> &getVaccinatieCentra() const;

public slots:

    void start();

    void stop();

    void nextDay();

    void previousDay();

    signals:
        void dayNrChanged(int dayNr);
        void endSimulation(int dayNr);

private:

    int day = 0;
    vector<Hub*> hubs;
    vector<VaccinatieCentrum*> vaccinatieCentra;
    string filename1;
    string filename2;
    QTimer *qTimer;
};


#endif //PSE_VACCINSIMULATIE_H
