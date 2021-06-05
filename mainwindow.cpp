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

    pieSeries = new QPieSeries();
    pieSeries->append("Niet gevaccineerd", 100);
    pieSeries->append("half gevaccineerd", 0);
    pieSeries->append("volledig gevaccineerd", 0);
    pieSeries->setLabelsVisible();
    pieSeries->setLabelsPosition(QPieSlice::LabelInsideHorizontal);

    currentDay = 0;
    simDay = 0;

    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Vaccinaties");
//    setDisabled(chart->legend());

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
    //QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), sim, SLOT(previousDay()));
    QObject::connect(ui->SpeedSlider, SIGNAL(sliderMoved(int)), sim, SLOT(updateSpeed(int)));
    QObject::connect(&StatisticsSingleton::getInstance(), SIGNAL(dataChange()), this, SLOT(dataChanged()));
    QObject::connect(sim, SIGNAL(dayNrChanged(int)), this, SLOT(changeDay(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), ui->stackedWidget,
                     &QStackedWidget::setCurrentIndex);

    QObject::connect(ui->NextDayButton_2, SIGNAL(clicked()), this, SLOT(nextDay()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), this, SLOT(previousDay()));
    QObject::connect(ui->ReturnButton, SIGNAL(clicked()), this, SLOT(returnToCurrent()));

    QVBoxLayout *layout = new QVBoxLayout();

    vector<Hub*> hubs = sim->getHubs();

    int i = 1;
    for(auto it = hubs.begin(); it != hubs.end(); it++){
        QPushButton *but = new QPushButton(QString::fromStdString("Hub"+to_string(i)));
        layout->addWidget(but);

        //pop op venster voor info van centrum
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle(QString::fromStdString("Hub"+to_string(i)));
        QGridLayout *grid = new QGridLayout;

        QLabel *vaccins = new QLabel("Vaccines:");
        grid->addWidget(vaccins,0,0);
        QLabel *count = new QLabel("Count:");
        grid->addWidget(count,0,1);

        int j = 1;
        map<string, Vaccin*> v = (*it)->getVaccins();
        for(map<string, Vaccin*>::iterator it2 = v.begin(); it2 != v.end(); it2++){
            QLabel *type = new QLabel(QString::fromStdString((*it2).first));
            grid->addWidget(type,j,0);
            QLabel *count = new QLabel(QString::fromStdString(to_string((*it2).second->aantal)));
            grid->addWidget(count,j,1);
            vaccineCount[(*it2).first] = count;
            QObject::connect((*it), SIGNAL(changeVaccinCount(std::string,int)), this, SLOT(setVaccinCount(std::string,int)));
            j++;
        }

        dialog->setLayout(grid);
        QObject::connect(but, SIGNAL(pressed()), dialog, SLOT(exec()));
        i++;
    }
    vector<VaccinatieCentrum*> c = sim->getVaccinatieCentra();
    for (vector<VaccinatieCentrum *>::iterator it = c.begin(); it != c.end(); it++) {
        centra[(*it)->getKfname()] = *it;
        string name = (*it)->getKfname();
        QPushButton *but = new QPushButton(QString::fromStdString(name));
        layout->addWidget(but);
        QProgressBar *progress = new QProgressBar();
        progress->setMaximum((*it)->getKaantalInwoners());
        progress->setMinimum(0);
        layout->addWidget(progress);
        QObject::connect(*it, SIGNAL(changeMainProgressBar(int)), progress, SLOT(setValue(int)));

        //pop op venster voor info van centrum
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle(QString::fromStdString(name));
        QGridLayout *grid = new QGridLayout;

        QLabel *inwoners = new QLabel("inhabitants:");
        grid->addWidget(inwoners,0,0);
        QLabel *inwo = new QLabel(QString::fromStdString(to_string((*it)->getKaantalInwoners())));
        grid->addWidget(inwo,0,1);

        QLabel *gevaccineerd = new QLabel("Percentage:");
        grid->addWidget(gevaccineerd,1,1);

        QLabel *leveren = new QLabel("Distribute vaccines:");
        grid->addWidget(leveren,1,2);
        QObject::connect(ui->StopButton, SIGNAL(clicked()), leveren, SLOT(show()));
        QObject::connect(ui->StartButton, SIGNAL(clicked()), leveren, SLOT(hide()));
        QObject::connect(ui->ReturnButton, SIGNAL(clicked()), leveren, SLOT(show()));
        QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), leveren, SLOT(hide()));

        layouts[name] = grid;

        dialog->setLayout(grid);
        QObject::connect(but, SIGNAL(pressed()), dialog, SLOT(exec()));
        QObject::connect((*it), SIGNAL(setVaccinInDialog(const std::string&, const Vaccin*, int)), this,
                         SLOT(addVaccin(const std::string&,const Vaccin*,int)));
    }

    ui->tabWidget->currentWidget()->setLayout(layout);
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::changeDay(int day) {
    simDay = day;
    currentDay = day;
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
    pieSeries->slices().at(0)->setValue(pRest);
    pieSeries->slices().at(0)->setLabel(QString("%1%").arg(100 * pieSeries->slices().at(0)->percentage(), 0, 'f', 1));
    pieSeries->slices().at(1)->setValue(pEerstePrikken);
    pieSeries->slices().at(1)->setLabel(QString("%1%").arg(100 * pieSeries->slices().at(1)->percentage(), 0, 'f', 1));
    pieSeries->slices().at(2)->setValue(pVolledigeVaccinaties);
    pieSeries->slices().at(2)->setLabel(QString("%1%").arg(100 * pieSeries->slices().at(2)->percentage(), 0, 'f', 1));

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

void MainWindow::addVaccin(const std::string &centrum, const Vaccin* vaccin, int i) {
    QLabel *VaccineName = new QLabel(QString::fromStdString(vaccin->type));
    layouts[centrum]->addWidget(VaccineName,i+1,0);
    QProgressBar *VaccinBar = new QProgressBar();
    progressBars[centrum][vaccin->type] = VaccinBar;
    layouts[centrum]->addWidget(VaccinBar,i+1,1);
    QSlider *vaccinSlider = new QSlider(Qt::Horizontal);
    QLabel *value = new QLabel("0");
    int teLeverenvaccins = round((centra[centrum]->getMaxStock()-centra[centrum]->getTotaalAantalVaccins())/vaccin->transport)*vaccin->transport;
    vaccinSlider->setMaximum(teLeverenvaccins);
    QObject::connect(vaccinSlider, &QSlider::valueChanged, this, [=] () {
        int v =  round(vaccinSlider->value()/vaccin->transport)*vaccin->transport;
        vaccinSlider->setValue(v);
        value->setText(QString::number(v));
    });
    layouts[centrum]->addWidget(vaccinSlider,i+1,2);
    layouts[centrum]->addWidget(value,i+1,3);

    QObject::connect(ui->ReturnButton, SIGNAL(clicked()), vaccinSlider, SLOT(show()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), vaccinSlider, SLOT(hide()));
    QObject::connect(ui->StopButton, SIGNAL(clicked()), vaccinSlider, SLOT(show()));
    QObject::connect(ui->StartButton, SIGNAL(clicked()), vaccinSlider, SLOT(hide()));

    QObject::connect(ui->ReturnButton, SIGNAL(clicked()), value, SLOT(show()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), value, SLOT(hide()));
    QObject::connect(ui->StopButton, SIGNAL(clicked()), value, SLOT(show()));
    QObject::connect(ui->StartButton, SIGNAL(clicked()), value, SLOT(hide()));

    QObject::connect(centra[centrum], SIGNAL(changeVaccinProgressBar(const std::string&,const std::string&,int)), this, SLOT(setVaccinValue(const std::string&,const std::string&,int)));
}

void MainWindow::setVaccinValue(const std::string &centrum,const std::string &vaccin,int value){
    progressBars[centrum][vaccin]->setValue(value);
}

void MainWindow::setVaccinCount(string vaccin, int count){
    vaccineCount[vaccin]->setText(QString::fromStdString(to_string(count)));
}

void MainWindow::nextDay(){
    currentDay++;

}

void MainWindow::previousDay(){
    currentDay--;

}

void MainWindow::returnToCurrent(){
    currentDay = simDay;

}