#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QtWidgets>
#include <QChartView>
#include <QPieSeries>
#include <map>
#include <string>
#include <QDialog>
#include <map>
#include "Hub.h"
#include "SimulationImporter.h"

#include "VaccinatieCentrum.h"

class VaccinSimulatie;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT


public:

    explicit MainWindow(VaccinSimulatie *sim, QWidget *parent = nullptr);

//    void setSimulation(VaccinSimulatie* s);
    ~MainWindow();

public slots:

    /*!
     * voegt een vaccin toe aan een hub
     * @param centrum centrum naam
     * @param vaccin pointer naar vaccin
     * @param centrumnr index van het centrum
     */
    void addVaccin(const std::string &centrum, Vaccin *vaccin, int centrumnr);

    /*!
     * verandert de dag
     * @param day de dag naar waar we veranderen
     */
    void changeDay(int day);

    /*!
     * bëeindigt de simulatie
     * @param day laatste dag
     */
    void endOfSimulation(int day);

    /*!
     * update de statastieken in de gui;
     */
    void updateStatistics();

    /*!
     * set het aantal vaccins in een centrum
     * @param centrum naam van het centrum
     * @param vaccin naam van het vaccin
     * @param value waarde waarnaar verandert word
     */
    void setVaccinValue(const std::string &centrum, const std::string &vaccin, int value);

    /*!
     * het aantal vaccins da er in da hub
     */
    void setVaccinCount(Hub *h, std::string vaccin, int count);

    /*!
     * toont de volgende dag op het scherm wanneer je in het verleden zit
     *
     */
    void nextDay();

    /*!
     * zet de vorige dag op het scherm
     */
    void previousDay();

    /*!
     * gaat terug naar de huidige dag
     */
    void returnToCurrent();

private:
    /*!
     * zet de dag van de gui
     * @param day waarde
     */
    void setGuiDay(int day);

    /*!
     * verandert de data van alle sliders en progress bars voor vorige dag
     * @emit emit void VaccinatieCentrum::changeMainProgressBar(int)
     * @emit void VaccinatieCentrum::changeVaccinProgressBar(const std::string&, const std::string&, int)
     * @emit void Hub::changeVaccinCount(std::string, int)
     */
    void changeData();

    int dayOffset;
    int simDay;
    std::map<std::string, VaccinatieCentrum *> centra;
    std::vector<Hub *> hubs;

    std::map<std::string, QGridLayout *> layouts;
    std::map<std::string, QPushButton *> commits;
    std::map<std::string, std::map<std::string, QProgressBar *>> progressBars;
    std::map<Hub*,std::map<std::string, QLabel*> > vaccineCount;

    Ui::MainWindow *ui;
    QPieSeries *pieSeries;

    QChartView *lineChartView;
    std::map<std::string, QLineSeries *> series;
};

#endif // MAINWINDOW_H
