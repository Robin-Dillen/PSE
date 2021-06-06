#include "mainwindow.h"
#include "cmake-build-debug/PSE_autogen/include/ui_mainwindow.h"
#include "VaccinSimulatie.h"
#include "StatisticsSingleton.h"
#include "Slider.h"

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
//    pieSeries->setLabelsVisible();
//    pieSeries->setLabelsPosition(QPieSlice::LabelInsideHorizontal);

    dayOffset = 0;
    simDay = 1;

    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Vaccinaties");
//    chart->legend()->hide();

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
    QObject::connect(ui->SpeedSlider, SIGNAL(sliderMoved(int)), sim, SLOT(updateSpeed(int)));
    QObject::connect(&StatisticsSingleton::getInstance(), SIGNAL(dataChange()), this, SLOT(dataChanged()));
    QObject::connect(sim, SIGNAL(dayNrChanged(int)), this, SLOT(changeDay(int)));
    QObject::connect(sim, SIGNAL(endSimulation(int)), this, SLOT(endOfSimulation(int)));
    QObject::connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), ui->stackedWidget,
                     &QStackedWidget::setCurrentIndex);

    QObject::connect(ui->NextDayButton_2, SIGNAL(clicked()), this, SLOT(nextDay()));
    QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), this, SLOT(previousDay()));
    QObject::connect(ui->ReturnButton, SIGNAL(clicked()), this, SLOT(returnToCurrent()));

    QVBoxLayout *layout = new QVBoxLayout();

    vector<Hub*> h = sim->getHubs();
    hubs = h;

    int i = 1;
    for(auto it = hubs.begin(); it != hubs.end(); it++){
        QPushButton *but = new QPushButton(QString::fromStdString("Hub"+to_string(i)));
        //but->setGeometry(300,100,100,50);
        //ui->tabWidget->layout()->addWidget(but);
        //this->layout()->addWidget(but);
        layout->addWidget(but);

        //pop op venster voor info van centrum
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle(QString::fromStdString("Hub"+to_string(i)));
        QGridLayout *grid = new QGridLayout;

        QLabel *vaccins = new QLabel("Vaccines:");
        grid->addWidget(vaccins,0,0);
        QLabel *countText = new QLabel("Count:");
        grid->addWidget(countText,0,1);

        int j = 1;
        map<string, Vaccin*> v = (*it)->getVaccins();
        for(map<string, Vaccin*>::iterator it2 = v.begin(); it2 != v.end(); it2++){
            QLabel *type = new QLabel(QString::fromStdString((*it2).first));
            grid->addWidget(type,j,0);
            QLabel *count = new QLabel(QString::fromStdString(to_string((*it2).second->aantal)));
            grid->addWidget(count,j,1);
            vaccineCount[(*it)][(*it2).first] = count;
            QObject::connect((*it), SIGNAL(changeVaccinCount(Hub*,std::string,int)), this, SLOT(setVaccinCount(Hub*,std::string,int)));
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

        int hubcount = 2;
        QPushButton *commit = new QPushButton("Commit");
        QObject::connect((*it), SIGNAL(newDay()), commit, SLOT(show()));
        QObject::connect(commit, SIGNAL(clicked()), commit, SLOT(hide()));
        QObject::connect(commit, SIGNAL(clicked()), commit, SLOT(hide()));
        for(int j = 0; j < (int) hubs.size(); j++){
            if(hubs[j]->getFverbondenCentra().find((*it)->getKfname()) != hubs[j]->getFverbondenCentra().end()){
                QLabel *leveren = new QLabel(QString::fromStdString("Hub"+to_string(j+1)+": "));
                grid->addWidget(leveren,1,(hubcount));
                QObject::connect(ui->StopButton, SIGNAL(clicked()), leveren, SLOT(show()));
                QObject::connect(ui->StartButton, SIGNAL(clicked()), leveren, SLOT(hide()));
                QObject::connect(ui->ReturnButton, SIGNAL(clicked()), leveren, SLOT(show()));
                QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), leveren, SLOT(hide()));
                hubcount += 2;
            }
        }
        commits[name] = commit;
        grid->addWidget(commit,0,hubcount);
        layouts[name] = grid;



        dialog->setLayout(grid);
        QObject::connect(but, SIGNAL(pressed()), dialog, SLOT(exec()));
        QObject::connect((*it), SIGNAL(setVaccinInDialog(const std::string&, Vaccin*, int)), this,
                         SLOT(addVaccin(const std::string&,Vaccin*,int)));
    }


    ui->tabWidget->currentWidget()->setLayout(layout);
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::changeDay(int day) {
    simDay = day;
    setGuiDay(day);
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
//    pieSeries->slices().at(0)->setLabel(QString("%1%").arg(100 * pieSeries->slices().at(0)->percentage(), 0, 'f', 1));
    pieSeries->slices().at(1)->setValue(pEerstePrikken);
//    pieSeries->slices().at(1)->setLabel(QString("%1%").arg(100 * pieSeries->slices().at(1)->percentage(), 0, 'f', 1));
    pieSeries->slices().at(2)->setValue(pVolledigeVaccinaties);
//    pieSeries->slices().at(2)->setLabel(QString("%1%").arg(100 * pieSeries->slices().at(2)->percentage(), 0, 'f', 1));

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

void MainWindow::addVaccin(const std::string &centrum, Vaccin* vaccin, int i) {
    QLabel *VaccineName = new QLabel(QString::fromStdString(vaccin->type));
    layouts[centrum]->addWidget(VaccineName,i+1,0);
    QProgressBar *VaccinBar = new QProgressBar();
    progressBars[centrum][vaccin->type] = VaccinBar;
    layouts[centrum]->addWidget(VaccinBar,i+1,1);

    int k = 2;
    for(int j = 0; j< (int) hubs.size(); j++) {
        if (hubs[j]->getFverbondenCentra().find(centrum) != hubs[j]->getFverbondenCentra().end()) {

            Slider *vaccinSlider = new Slider(vaccin->transport, hubs[i], centra[centrum], vaccin);
            QLabel *value = new QLabel("0");

            int totalVaccines = 0;
            for (int l = 0; l < (int) hubs.size(); l++) {
                totalVaccines += hubs[l]->getAllVaccins(vaccin);
            }
            int teLeverenvaccins = floor((centra[centrum]->getMaxStock() - centra[centrum]->getTotaalAantalVaccins()) /
                                         vaccin->transport) * vaccin->transport;
            int maxteVerdelen = min(teLeverenvaccins, totalVaccines);
            vaccinSlider->setMaximum(maxteVerdelen);

            QObject::connect(vaccinSlider, &Slider::valueChanged, vaccinSlider, &Slider::changeValue);
            QObject::connect(vaccinSlider, SIGNAL(changeText(const QString&)), value, SLOT(setText(const QString &)));

            layouts[centrum]->addWidget(vaccinSlider, i + 1, k);
            layouts[centrum]->addWidget(value, i + 1, k + 1);
            k += 2;

            QObject::connect(ui->ReturnButton, SIGNAL(clicked()), vaccinSlider, SLOT(show()));
            QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), vaccinSlider, SLOT(hide()));
            QObject::connect(ui->StopButton, SIGNAL(clicked()), vaccinSlider, SLOT(show()));
            QObject::connect(ui->StartButton, SIGNAL(clicked()), vaccinSlider, SLOT(hide()));
            QObject::connect(commits[centrum], SIGNAL(clicked()), vaccinSlider, SLOT(hide()));
            QObject::connect(centra[centrum], SIGNAL(newDay()), vaccinSlider, SLOT(show()));
            QObject::connect(centra[centrum], &VaccinatieCentrum::newDay, vaccinSlider, &Slider::resetSlider);
            QObject::connect(centra[centrum], &VaccinatieCentrum::newDay, vaccinSlider, &Slider::changeMaximum);

            QObject::connect(commits[centrum], SIGNAL(clicked()), vaccinSlider, SLOT(sendVaccins()));

            QObject::connect(ui->ReturnButton, SIGNAL(clicked()), value, SLOT(show()));
            QObject::connect(ui->PreviousDayButton, SIGNAL(clicked()), value, SLOT(hide()));
            QObject::connect(ui->StopButton, SIGNAL(clicked()), value, SLOT(show()));
            QObject::connect(ui->StartButton, SIGNAL(clicked()), value, SLOT(hide()));
            QObject::connect(commits[centrum], SIGNAL(clicked()), value, SLOT(hide()));
            QObject::connect(centra[centrum], SIGNAL(newDay()), value, SLOT(show()));


        }
    }
    QObject::connect(centra[centrum], SIGNAL(changeVaccinProgressBar(const std::string&,const std::string&,int)), this, SLOT(setVaccinValue(const std::string&,const std::string&,int)));
}

void MainWindow::setVaccinValue(const std::string &centrum,const std::string &vaccin,int value){
    progressBars[centrum][vaccin]->setValue(value);
}

void MainWindow::setVaccinCount(Hub* h, string vaccin, int count){
    vaccineCount[h][vaccin]->setText(QString::fromStdString(to_string(count)));
}

void MainWindow::setGuiDay(int day){
    string daytext = "day: " + to_string(day);
    QString time = QString::fromStdString(daytext);
    ui->DayText->setText(time);
}

void MainWindow::nextDay(){
    if(dayOffset == 0){
        ui->ReturnButton->click();
        ui->NextDayButton->click();
    }else{
        dayOffset--;
        setGuiDay(simDay-dayOffset);
        changeData();
    }
}

void MainWindow::previousDay(){
    if(simDay-dayOffset != 1){
        dayOffset++;
        setGuiDay(simDay-dayOffset);
        changeData();
    }
}

void MainWindow::changeData() {
    StatisticsSingleton &stats = StatisticsSingleton::getInstance();
    string file = "../SavedData/dag" + to_string(simDay - dayOffset) + ".xml";
    SimulationImporter importer(file);
    int centrumnr = 0;
    for (map<string, VaccinatieCentrum *>::iterator centrumIt = centra.begin();
         centrumIt != centra.end(); centrumIt++) {
        emit (*centrumIt).second->changeMainProgressBar(importer.getAantalVaccinatiesCentrum(centrumnr));
        const map<string, pair<Vaccin *, int>> &vaccins = (*centrumIt).second->getAantalVaccins1();
        for (map<string, pair<Vaccin *, int> >::const_iterator vaccinIterator = vaccins.begin();
             vaccinIterator != vaccins.end(); vaccinIterator++) {
            int percentage = 0;
            if (importer.getAantalVaccinatiesCentrum(centrumnr) != 0) {
                percentage = (importer.getCentrumVaccinCount(centrumnr, (*vaccinIterator).first) * 100 /
                              importer.getAantalVaccinatiesCentrum(centrumnr));
            }
            emit (*centrumIt).second->changeVaccinProgressBar((*centrumIt).second->getKfname(), (*vaccinIterator).first,
                                                              percentage);
        }
    }
    int i = 0;
    for (std::vector<Hub *>::iterator hubIterator = hubs.begin(); hubIterator != hubs.end(); hubIterator++) {
        const map<string, Vaccin *> &vaccins = (*hubIterator)->getVaccins();
        for (map<string, Vaccin *>::const_iterator vaccinIterator = vaccins.begin();
             vaccinIterator != vaccins.end(); vaccinIterator++) {
            emit (*hubIterator)->changeVaccinCount((*hubIterator),(*vaccinIterator).first,
                                                   importer.getHubVaccinCount(i,vaccinIterator->first));
        }
        ++i;
    }

    //pie chart
    float totaal = stats.getTotaalAantalMensen();
    int totaal_eersteprik = 0;
    int totaal_tweedeprik = 0;
    i = 0;
    for (map<string, VaccinatieCentrum *>::iterator centrumIt = centra.begin();
         centrumIt != centra.end(); centrumIt++) {
        totaal_eersteprik += importer.getTotaalAantalEerstePrikken(i);
        totaal_tweedeprik += importer.getAantalVaccinatiesCentrum(i);
        ++i;
    }

    float pEerstePrikken = (totaal_eersteprik / totaal) * 100;
    float pVolledigeVaccinaties = (totaal_tweedeprik / totaal) * 100;
    float pRest = 100 - (pEerstePrikken + pVolledigeVaccinaties);
    pieSeries->slices().at(0)->setValue(pRest);
    pieSeries->slices().at(1)->setValue(pEerstePrikken);
    pieSeries->slices().at(2)->setValue(pVolledigeVaccinaties);

}

void MainWindow::returnToCurrent(){
    dayOffset = 0;
    setGuiDay(simDay-dayOffset);
}