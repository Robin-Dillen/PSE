#include "mainwindow.h"
#include "cmake-build-debug/PSE_autogen/include/ui_mainwindow.h"
#include "VaccinSimulatie.h"
#include "StatisticsSingleton.h"
#include<iostream>

#include <QtCharts>
#include <QtWidgets>
#include <QChartView>
#include <QPieSeries>
#include <QProgressBar>

MainWindow::MainWindow(VaccinSimulatie *sim, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    pieChart = new QPieSeries();
    pieChart->append("Niet gevaccineert", 100);
    pieChart->append("half gevaccineert", 0);
    pieChart->append("volledig gevaccineert", 0);

    QChart *chart = new QChart();
    chart->addSeries(pieChart);
    chart->setTitle("Test");

    QChartView *view = new QChartView(chart);
    view->setParent(ui->horizontalFrame_page1);

    QObject::connect(ui->StartButton, SIGNAL(clicked()), sim, SLOT(start()));
    QObject::connect(ui->StopButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->NextDayButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->NextDayButton, SIGNAL(clicked()), sim, SLOT(nextDay()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), sim, SLOT(previousDay()));
    QObject::connect(&StatisticsSingleton::getInstance(), SIGNAL(dataChange()), this, SLOT(dataChanged()));
    QObject::connect(sim, &VaccinSimulatie::dayNrChanged, this, &MainWindow::changeDay);
    QObject::connect(sim, &VaccinSimulatie::endSimulation, this, &MainWindow::endOfSimulation);

    QVBoxLayout *layout = new QVBoxLayout(this);

    vector<Hub*> hubs = sim->getHubs();
    int yPos = 50;
    for(auto it = hubs.begin(); it != hubs.end(); it++){
        QPushButton *but = new QPushButton("Hub");
        //but->setGeometry(200,yPos,50,20);
        layout->addWidget(but);
        yPos += 30;
    }
    vector<VaccinatieCentrum*> centra = sim->getVaccinatieCentra();
    for(auto it = centra.begin(); it != centra.end(); it++){
        string name = (*it)->getKfname();
        QPushButton *but = new QPushButton(QString::fromStdString(name));
        //but->setGeometry(200,yPos,50,20);
        layout->addWidget(but);
        QProgressBar *progress = new QProgressBar();
        progress->setMaximum((*it)->getKaantalInwoners());
        progress->setMinimum(0);
        layout->addWidget(progress);
        QObject::connect((*it), &VaccinatieCentrum::changeProgressBar, this, SLOT(progress->setValue()));
        yPos += 30;
    }

    ui->tabWidget->currentWidget()->setLayout(layout);
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

void MainWindow::dataChanged() const {
    StatisticsSingleton &stats = StatisticsSingleton::getInstance();
    float totaal = stats.getTotaalAantalMensen();
    float pEerstePrikken = stats.getTotaalEerstePrikken() / totaal;
    float pVolledigeVaccinaties = stats.getTotaalVolledigeVaccinaties() / totaal;
    float pRest = 100 - (pEerstePrikken + pVolledigeVaccinaties);
    pieChart->slices().at(0)->setValue(pRest);
    pieChart->slices().at(1)->setValue(pEerstePrikken);
    pieChart->slices().at(2)->setValue(pVolledigeVaccinaties);
}
