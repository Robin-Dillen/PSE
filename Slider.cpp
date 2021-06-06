//
// Created by Niels on 6/06/2021.
//

#include "Slider.h"


Slider::Slider(int i) {
    this->setOrientation(Qt::Horizontal);
    interval = i;
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


