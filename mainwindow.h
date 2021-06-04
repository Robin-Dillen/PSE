#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void changeDay(int day);

    void endOfSimulation(int day);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
//    VaccinSimulatie* simulatie;

};

#endif // MAINWINDOW_H
