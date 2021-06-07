//
// Created by Niels on 6/06/2021.
//

#include "Slider.h"
#include "lib/DesignByContract.h"
#include <QDialog>
#include <QLayout>
#include <QLabel>

Slider::Slider(int i, Hub *h,int nummer, VaccinatieCentrum *c, Vaccin *v) : QSlider(Qt::Horizontal), _initCheck(this) {
    REQUIRE(h->isProperlyInitialized(), "De hub is niet goed geïnitialiseert!");
    REQUIRE(c->isProperlyInitialized(), "het centrum is niet goed geïnitialiseert!");
    REQUIRE(v->isProperlyInitialized(), "het vaccin is niet goed geïnitialiseert!");
    interval = i;
    hub = h;
    hubnr = nummer;
    centrum = c;
    vaccin = v;
    ENSURE(isProperlyInitialized(), "Object isn't properly initialized when exiting constructor!");
}

bool Slider::isProperlyInitialized() const {
    return _initCheck == this;
}

void Slider::resetSlider() {
    REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
    this->setValue(0);
    const QString s = QString::number(0);
    emit changeText(s);
    ENSURE(hub->isProperlyInitialized(), "De hub is niet goed geïnitialiseert!");
    ENSURE(centrum->isProperlyInitialized(), "het centrum is niet goed geïnitialiseert!");
    ENSURE(vaccin->isProperlyInitialized(), "het vaccin is niet goed geïnitialiseert!");
}

void Slider::changeValue() {
    REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
    int v = round(this->value() / interval) * interval;
    this->setValue(v);
    const QString s = QString::number(v);
    emit changeText(s);
    ENSURE(hub->isProperlyInitialized(), "De hub is niet goed geïnitialiseert!");
    ENSURE(centrum->isProperlyInitialized(), "het centrum is niet goed geïnitialiseert!");
    ENSURE(vaccin->isProperlyInitialized(), "het vaccin is niet goed geïnitialiseert!");
}


void Slider::changeMaximum() {
    REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
    int totalVaccines = hub->getAllVaccins(vaccin);
    int teLeverenvaccins;

    if (vaccin->temperatuur < 0) {
        teLeverenvaccins = floor((centrum->getKcapaciteit() - centrum->getTotaalAantalVaccins() -
                                  centrum->getTotaalAantalGeleverdeVaccins()) / interval) * interval;
    } else {
        teLeverenvaccins = floor((centrum->getMaxStock() - centrum->getTotaalAantalVaccins() -
                                  centrum->getTotaalAantalGeleverdeVaccins()) / interval) * interval;
    }
    int maxteVerdelen = min(teLeverenvaccins, totalVaccines);
    this->setMaximum(maxteVerdelen);
    ENSURE(hub->isProperlyInitialized(), "De hub is niet goed geïnitialiseert!");
    ENSURE(centrum->isProperlyInitialized(), "het centrum is niet goed geïnitialiseert!");
    ENSURE(vaccin->isProperlyInitialized(), "het vaccin is niet goed geïnitialiseert!");
}

void Slider::sendVaccins() {
    REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
    ENSURE(hub->isProperlyInitialized(), "De hub is niet goed geïnitialiseert!");
    ENSURE(centrum->isProperlyInitialized(), "het centrum is niet goed geïnitialiseert!");
    ENSURE(vaccin->isProperlyInitialized(), "het vaccin is niet goed geïnitialiseert!");
    int maxteLeveren;
    if (vaccin->temperatuur < 0) {
        maxteLeveren = centrum->getKcapaciteit();
    }else{
        maxteLeveren = centrum->getMaxStock();
    }
    bool teveel = false;
    while(centrum->getTotaalAantalVaccins()+centrum->getTotaalAantalGeleverdeVaccins()+this->value() > maxteLeveren && this->value() > 0){
        teveel = true;
        this->setValue(this->value()-interval);
    }
    if(teveel){
        QDialog dialog(this);
        dialog.setWindowTitle("Distributed Too much vaccines");
        QVBoxLayout layout;
        dialog.setLayout(&layout);
        string t = "Distributed too much vaccines from hub"+ to_string(hubnr) + " with vaccine " + vaccin->type + ". New value: " +
                to_string(this->value());
        QLabel text(QString::fromStdString(t));
        layout.addWidget(&text);
        dialog.exec();
    }
    hub->distributeManual(vaccin->type, this->value());
    centrum->ontvangLevering(this->value(), vaccin);

}



