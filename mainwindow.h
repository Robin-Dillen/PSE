#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts>
#include <QtWidgets>
#include <QChartView>
#include <QPieSeries>

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

    void endOfSimulation(int day);

public slots:

    void dataChanged() const;

    void changeDay(int day);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPieSeries *pieChart;
//    VaccinSimulatie* simulatie;

};

#endif // MAINWINDOW_H
