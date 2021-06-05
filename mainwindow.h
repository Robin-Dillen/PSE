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

    void addVaccin(const std::string &centrum, const Vaccin* vaccin, int i);

    void changeDay(int day);

    void endOfSimulation(int day);

    void dataChanged();

    void setVaccinValue(const std::string &centrum,const std::string &vaccin, int value);

    void setVaccinCount(std::string vaccin,int count);

    void nextDay();

    void previousDay();

    void returnToCurrent();

private:
    int currentDay;
    int simDay;
    std::map<std::string, VaccinatieCentrum*> centra;
    std::vector<Hub*> hubs;


    std::map<std::string, QGridLayout *> layouts;

    std::map<std::string, std::map<std::string, QProgressBar*>> progressBars;
    std::map<std::string, QLabel*> vaccineCount;
    Ui::MainWindow *ui;
    QPieSeries *pieSeries;

    QChartView *lineChartView;
    std::map<std::string, QLineSeries *> series;
//    VaccinSimulatie* simulatie;

};

#endif // MAINWINDOW_H
