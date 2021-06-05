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

    void changeDay(int day);

    void endOfSimulation(int day);

    void dataChanged();

    void addVaccines(const std::string &centrum, const std::pair<std::string, int> &vaccines);

private:

    std::map<std::string, QDialog*> dialogs;
    Ui::MainWindow *ui;
    QPieSeries *pieChart;

    QChartView *lineChartView;
    std::map<std::string, QLineSeries *> series;
//    VaccinSimulatie* simulatie;

};

#endif // MAINWINDOW_H