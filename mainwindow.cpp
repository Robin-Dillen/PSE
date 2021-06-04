#include "mainwindow.h"
#include "cmake-build-debug/PSE_autogen/include/ui_mainwindow.h"
#include "VaccinSimulatie.h"
#include<iostream>

#include <QtCharts>
#include <QtWidgets>
#include <QChartView>
#include <QPieSeries>

MainWindow::MainWindow(VaccinSimulatie *sim, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QPieSeries *series = new QPieSeries();
    series->append("Niet gevaccineert", 100);
    series->append("half gevaccineert", 0);
    series->append("volledig gevaccineert", 0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Test");

    QChartView *view = new QChartView(chart);
    view->setParent(ui->horizontalFrame_page1);

    QObject::connect(ui->StartButton, SIGNAL(clicked()), sim, SLOT(start()));
    QObject::connect(ui->StopButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->NextDayButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->NextDayButton, SIGNAL(clicked()), sim, SLOT(nextDay()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), sim, SLOT(previousDay()));
    /*QLabel *label = new QLabel("centum 0");
    label->setGeometry(100,100,100,50);
    */
}

//void MainWindow::setSimulation(VaccinSimulatie* s){
//    simulatie = s;
//}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startSimulation() {
//    simulatie->start();
}

void MainWindow::stopSimulation(){
//  simulatie->stop();
}

void MainWindow::nextDay(){
//    simulatie->nextDay();
}

void MainWindow::previousDay(){
//    simulatie->previousDay();
}


void MainWindow::changeDay(int day) {
    string daytext = "day: " + to_string(day);
    QString time = QString::fromStdString(daytext);
    ui->DayText->setText(time);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void MainWindow::endOfSimulation(int day) {
    string daytext = "vaccination ended at day: " + to_string(day);
    QString time = QString::fromStdString(daytext);
    ui->DayText->setText(time);
}
