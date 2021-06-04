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
#include <QDialog>

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
    QObject::connect(sim, SIGNAL(dayNrChanged(int)), this,SLOT(changeDay(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));

    QVBoxLayout *layout = new QVBoxLayout();

    vector<Hub*> hubs = sim->getHubs();

    for(auto it = hubs.begin(); it != hubs.end(); it++){
        QPushButton *but = new QPushButton("Hub");
        layout->addWidget(but);

    }
    vector<VaccinatieCentrum*> centra = sim->getVaccinatieCentra();
    for (vector<VaccinatieCentrum *>::iterator it = centra.begin(); it != centra.end(); it++) {
        string name = (*it)->getKfname();
        QPushButton *but = new QPushButton(QString::fromStdString(name));
        layout->addWidget(but);
        QProgressBar *progress = new QProgressBar();
        progress->setMaximum((*it)->getKaantalInwoners());
        progress->setMinimum(0);
        layout->addWidget(progress);
        QObject::connect(*it, SIGNAL(changeProgressBar(int)), progress, SLOT(setValue(int)));

        //pop op venster voor info van centrum
        QDialog *dialog = new QDialog(this);
        QObject::connect(but, SIGNAL(pressed()), dialog, SLOT(exec()));
        QLabel *inwoners = new QLabel(QString::fromStdString("inhabitants: " + to_string((*it)->getKaantalInwoners())));
        inwoners->setParent(dialog);

//        QGroupBox *groupBox = new QGroupBox(QString::fromStdString(name));
//        QVBoxLayout *vbox = new QVBoxLayout;
//        QLabel *inwoners = new QLabel(QString::fromStdString("inhabitants: "+ to_string((*it)->getKaantalInwoners())));
//        vbox->addWidget(inwoners);
//        groupBox->setLayout(vbox);
//        groupBox->setGeometry(100,100,100,100);
//        ui->tabWidget->addTab(groupBox,QString::fromStdString(name));
//        groupBox->setVisible(false);
//        groupBox->hide();
    }

    ui->tabWidget->currentWidget()->setLayout(layout);
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

void MainWindow::endOfSimulation(int day) {
    string daytext = "vaccination ended at day: " + to_string(day);
    QString time = QString::fromStdString(daytext);
    ui->DayText->setText(time);
}

void MainWindow::dataChanged() const {
    StatisticsSingleton &stats = StatisticsSingleton::getInstance();
    float totaal = stats.getTotaalAantalMensen();
    float pEerstePrikken = (stats.getTotaalEerstePrikken() / totaal) * 100;
    float pVolledigeVaccinaties = (stats.getTotaalVolledigeVaccinaties() / totaal) * 100;
    float pRest = 100 - (pEerstePrikken + pVolledigeVaccinaties);
    pieChart->slices().at(0)->setValue(pRest);
    pieChart->slices().at(1)->setValue(pEerstePrikken);
    pieChart->slices().at(2)->setValue(pVolledigeVaccinaties);
}
