//
// Created by Niels on 2/06/2021.
//

#ifndef PSE_VACCINSIMULATIE_H
#define PSE_VACCINSIMULATIE_H

#include "Lib.h"

#include <QApplication>
#include <QTime>
#include <QTimer>

class Hub;

class VaccinatieCentrum;

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

    bool nextDay();

    void previousDay();

    void updateSpeed(int newspeed);

signals:

    void dayNrChanged(int dayNr);

    void endSimulation(int dayNr);

private:
    void addToFile();

    int day;
    vector<Hub *> hubs;
    vector<VaccinatieCentrum *> vaccinatieCentra;
    string filename1;
    string filename2;
    QTimer *qTimer;
};


#endif //PSE_VACCINSIMULATIE_H
