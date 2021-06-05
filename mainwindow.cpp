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

    ui->NextDayButton_2->hide();
    ui->ReturnButton->hide();

    pieChart = new QPieSeries();
    pieChart->append("Niet gevaccineert", 100);
    pieChart->append("half gevaccineert", 0);
    pieChart->append("volledig gevaccineert", 0);

    QChart *chart = new QChart();
    chart->addSeries(pieChart);
    chart->setTitle("Vaccinaties");

    QChartView *view = new QChartView(chart);
    view->setParent(ui->horizontalFrame_page1);
    view->setFixedSize(ui->horizontalFrame_page1->width(), ui->horizontalFrame_page1->height());

    //line chart
    StatisticsSingleton &stats = StatisticsSingleton::getInstance();
    QChart *line_chart = new QChart();

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 365);
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, stats.getTotaalAantalMensen() * 2);

    lineChartView = new QChartView(line_chart);
    lineChartView->setParent(ui->horizontalFrame_page2);
    lineChartView->chart()->legend()->hide();
    lineChartView->chart()->setTitle("Simple line chart example");
    lineChartView->chart()->addAxis(axisX, Qt::AlignBottom);
    lineChartView->chart()->addAxis(axisY, Qt::AlignLeft);
    lineChartView->setRubberBand(QChartView::RectangleRubberBand);
    lineChartView->chart()->legend()->setVisible(true);
    lineChartView->chart()->legend()->setAlignment(Qt::AlignBottom);
    lineChartView->chart()->setMargins(QMargins(0, 0, 0, 0));
    lineChartView->setFixedSize(ui->horizontalFrame_page2->width(), ui->horizontalFrame_page2->height());

    QObject::connect(ui->StartButton, SIGNAL(clicked()), sim, SLOT(start()));
    QObject::connect(ui->StopButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->NextDayButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->NextDayButton, SIGNAL(clicked()), sim, SLOT(nextDay()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), sim, SLOT(stop()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), sim, SLOT(previousDay()));
    QObject::connect(ui->SpeedSlider, SIGNAL(sliderMoved(int)), sim, SLOT(updateSpeed(int)));
    QObject::connect(&StatisticsSingleton::getInstance(), SIGNAL(dataChange()), this, SLOT(dataChanged()));
    QObject::connect(sim, SIGNAL(dayNrChanged(int)), this, SLOT(changeDay(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), ui->stackedWidget,
                     &QStackedWidget::setCurrentIndex);

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
        QVBoxLayout *vbox = new QVBoxLayout;

        QLabel *inwoners = new QLabel(QString::fromStdString("inhabitants: "));
        vbox->addWidget(inwoners);
        QLabel *inwo = new QLabel(QString::fromStdString(to_string((*it)->getKaantalInwoners())));
        vbox->addWidget(inwo);

        dialogs[name] = dialog;

        dialog->setLayout(vbox);
        QObject::connect(but, SIGNAL(pressed()), dialog, SLOT(exec()));
        QObject::connect((*it), SIGNAL(setVaccinesInDialog(string, pair<string, int>)), this, SLOT(addVaccines(string, pair<string, int>)));
    }

    ui->tabWidget->currentWidget()->setLayout(layout);
}


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

void MainWindow::dataChanged() {
    static int current_day = 0;
    static int k = 1;
    current_day++;
    StatisticsSingleton &stats = StatisticsSingleton::getInstance();

    //pie chart
    float totaal = stats.getTotaalAantalMensen();
    float pEerstePrikken = (stats.getTotaalEerstePrikken() / totaal) * 100;
    float pVolledigeVaccinaties = (stats.getTotaalVolledigeVaccinaties() / totaal) * 100;
    float pRest = 100 - (pEerstePrikken + pVolledigeVaccinaties);
    pieChart->slices().at(0)->setValue(pRest);
    pieChart->slices().at(1)->setValue(pEerstePrikken);
    pieChart->slices().at(2)->setValue(pVolledigeVaccinaties);

    //line chart
    map<string, int> totaal_geleverde_vaccins = stats.getGeleverdeVaccins();
    auto axes = lineChartView->chart()->axes();
    if (current_day >= 365 * k) {
        ++k;
        axes.front()->setRange(0, 365 * k);
    }
    for (map<string, int>::iterator vaccins = totaal_geleverde_vaccins.begin();
         vaccins != totaal_geleverde_vaccins.end(); ++vaccins) {
        if (series.find(vaccins->first) == series.end()) {
            series[vaccins->first] = new QLineSeries();
            series[vaccins->first]->setName(QString::fromStdString(vaccins->first));
            lineChartView->chart()->addSeries(series[vaccins->first]);
            series[vaccins->first]->attachAxis(axes.front());
            series[vaccins->first]->attachAxis(axes.back());
//            series[vaccins->first]->attachAxis(m_axis);
        }
        series[vaccins->first]->append(current_day, vaccins->second);
    }
}

void MainWindow::addVaccines(const string &centrum, const pair<string, int> &vaccines){
    map<string, int>::const_iterator it;
    QLabel *VaccineName = new QLabel(QString::fromStdString(vaccines.first));
    dialogs[centrum]->layout()->addWidget(VaccineName);
    QProgressBar *VaccineBar = new QProgressBar();
    dialogs[centrum]->layout()->addWidget(VaccineBar);
}
