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

class MainWindow;

class VaccinSimulatie : public QObject {
Q_OBJECT
public:
    VaccinSimulatie(vector<Hub *> &hubs, std::vector<VaccinatieCentrum *> &vaccinatie_centra,
                    const string &testfilename,
                    bool c_out = true);

    void setWindow(MainWindow *w);
//    void start();
//    void stop();
//    void nextDay();
//    void previousDay();

public slots:

    void start();

    void stop();

    void nextDay();

    void previousDay();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    /*!
     * pauzes the simulation for a given time length
     * @param time how long the simulation is pauzes (in micro secs)
     */
    void delay(int time);

    bool pause = true;
    MainWindow *window;
    int day = 0;
    vector<Hub*> hubs;
    vector<VaccinatieCentrum*> vaccinatieCentra;
    string filename1;
    string filename2;
    QTimer *qTimer;
};


#endif //PSE_VACCINSIMULATIE_H
