#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

MainWindow::MainWindow(VaccinSimulatie* s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    simulatie = s;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startSimulation(){
    simulatie->start();
}
