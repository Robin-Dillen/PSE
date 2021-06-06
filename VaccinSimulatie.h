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
    /*!
     * initialiseer de simulatie
     * @param hubs de hubs
     * @param vaccinatie_centra de centra
     * @param testfilename de filename van de test
     * \n REQUIRE(pos != string::npos, "Given argument doesn't include a test file!");
     * \n ENSURE(isProperlyInitialized(), "The object isn't properly initialized when exiting the constructor!");
     */
    VaccinSimulatie(vector<Hub *> &hubs, std::vector<VaccinatieCentrum *> &vaccinatie_centra,
                    const string &testfilename);

    /*!
     * geeft alle hubs van de simulatie terug
     * @return const vector<Hub *> &
     */
    const vector<Hub *> &getHubs() const;

    /*!
     * geeft alle centra van de simulatie terug
     * @return const vector<VaccinatieCentrum *> &
     * \n REQUIRE(isProperlyInitialized(), "The object isn't properly initialised when calling this function");
     */
    const vector<VaccinatieCentrum *> &getVaccinatieCentra() const;

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     * \n REQUIRE(isProperlyInitialized(), "The object isn't properly initialised when calling this function");
     */
    bool isProperlyInitialized() const;

    ~VaccinSimulatie() override;

public slots:

    /*!
     * start de simulatie
     * \n REQUIRE(isProperlyInitialized(), "The object isn't properly initialised when calling this function");
     * \n ENSURE(qTimer->isActive() == true, "Timer is niet succesvol gestart!");
     */
    void start();

    /*!
     * stopt de simualatie
     * \n REQUIRE(isProperlyInitialized(), "The object isn't properly initialised when calling this function");
     * \n ENSURE(qTimer->isActive() == false, "Timer is niet succesvol gestopt!");
     */
    void stop();

    /*!
     * doet 1 stap van de simulatie
     * @return geeft true terug als de simulatie niet is afgelopen anders false
     * @emit void VaccinSimulatie::dayNrChanged(int)
     * @emit void VaccinSimulatie::endSimulation(int)
     * @emit void StatisticsSingleton::dataChange()
     * \n REQUIRE(isProperlyInitialized(), "The object isn't properly initialised when calling this function");µ
     * \n ENSURE(FileExists("../SavedData/dag" + to_string(day + 1) + ".xml"), "De backup file is niet succesvol aangemaakt!");
     * \n ENSURE(!FileIsEmpty("../SavedData/dag" + to_string(day + 1) + ".xml"), "De backup file is niet correct geschreven!");
     */
    bool nextDay();

    /*!
     * verandert de snelheid van de simulatie
     * @param newspeed hoeveel ms er zijn tussen elke loop
     * \n REQUIRE(isProperlyInitialized(), "The object isn't properly initialised when calling this function");
     * \n ENSURE(qTimer->interval() == newspeed, "De snelheid is niet succesvol geüpdate");
     */
    void updateSpeed(int newspeed);

signals:

    /*!
     * stuurt een signaal dat er de dag nr is verandert
     * @param dayNr nieuwe dag
     */
    void dayNrChanged(int dayNr);

    /*!
     * stopt de simulatie
     * @param dayNr
     */
    void endSimulation(int dayNr);

private:

    const VaccinSimulatie *const _initCheck;
    int day;
    vector<Hub *> hubs;
    vector<VaccinatieCentrum *> vaccinatieCentra;
    string filename1;
    string filename2;
    QTimer *qTimer;
};


#endif //PSE_VACCINSIMULATIE_H
