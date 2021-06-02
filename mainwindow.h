#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void printSomething();

private:
    Ui::MainWindow *ui;
    bool pause = false;
};

#endif // MAINWINDOW_H
