#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class VaccinSimulatie;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:

    explicit MainWindow(QWidget *parent = nullptr);
    void setSimulation(VaccinSimulatie* s);
    ~MainWindow();
    void changeDay(int day);

private:
    Ui::MainWindow *ui;
    VaccinSimulatie* simulatie;

private slots:
        void startSimulation();
};

#endif // MAINWINDOW_H
