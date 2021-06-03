#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::setSimulation(VaccinSimulatie* s){
    simulatie = s;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startSimulation(){
    simulatie->start();
}
