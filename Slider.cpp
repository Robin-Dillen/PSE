//
// Created by Niels on 6/06/2021.
//

#include "Slider.h"
#include "lib/DesignByContract.h"


Slider::Slider(int i, Hub *h, VaccinatieCentrum *c, Vaccin *v) : QSlider(Qt::Horizontal), _initCheck(this) {
    interval = i;
    hub = h;
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
}

void Slider::changeValue() {
    REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
    int v = round(this->value() / interval) * interval;
    this->setValue(v);
    const QString s = QString::number(v);
    emit changeText(s);
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
}

void Slider::sendVaccins() {
    REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
    hub->distributeManual(vaccin->type, this->value());
    centrum->ontvangLevering(this->value(), vaccin);
}



