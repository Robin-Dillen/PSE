#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <VaccinSimulatie.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:

    explicit MainWindow(VaccinSimulatie* s, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VaccinSimulatie* simulatie;

private slots:
        void startSimulation();
};

#endif // MAINWINDOW_H
