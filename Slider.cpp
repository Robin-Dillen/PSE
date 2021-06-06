//
// Created by Niels on 6/06/2021.
//

#include "Slider.h"


Slider::Slider(int i, Hub *h, VaccinatieCentrum *c, Vaccin* v){
    this->setOrientation(Qt::Horizontal);
    interval = i;
    hub = h;
    centrum = c;
    vaccin = v;
}


void Slider::resetSlider(){
    this->setValue(0);
    const QString s = QString::number(0);
    emit changeText(s);
}

void Slider::changeValue(){
    int v =  round(this->value()/interval)*interval;
    this->setValue(v);
    const QString s = QString::number(v);
    emit changeText(s);
}


void Slider::changeMaximum(){
    int totalVaccines = hub->getAllVaccins(vaccin);
    int teLeverenvaccins;

    if(vaccin->temperatuur < 0){
        teLeverenvaccins = floor((centrum->getKcapaciteit()-centrum->getTotaalAantalVaccins()-centrum->getTotaalAantalGeleverdeVaccins())/interval)*interval;
    }else{
        teLeverenvaccins = floor((centrum->getMaxStock()-centrum->getTotaalAantalVaccins()-centrum->getTotaalAantalGeleverdeVaccins())/interval)*interval;
    }
    int maxteVerdelen = min(teLeverenvaccins, totalVaccines);
    this->setMaximum(maxteVerdelen);
}

void Slider::sendVaccins(){
    hub->distributeManual(vaccin->type, this->value());
    centrum->ontvangLevering(this->value(), vaccin);
}

