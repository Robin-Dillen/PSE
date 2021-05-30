//
// Created by Niels on 27/05/2021.
//

#ifndef PSE_MAINWINDOW_H
#define PSE_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

using namespace std;

class MainWindow: public QMainWindow {
public:
    MainWindow();
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
};


#endif //PSE_MAINWINDOW_H
