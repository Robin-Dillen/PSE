//
// Created by Niels on 27/05/2021.
//
#include "MainWindow.h"

MainWindow::MainWindow() {
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
}
