#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "VaccinSimulatie.h"
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

void MainWindow::changeDay(int day){
    string daytext = "day: "+to_string(day);
    QString time = QString::fromStdString(daytext);
    ui->DayText->setText(time);
}
