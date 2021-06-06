//
// Created by Niels on 6/06/2021.
//
#include <QSlider>
#include <math.h>
#include "VaccinatieCentrum.h"
#include "Hub.h"
#include "Vaccins.h"

#ifndef PSE_SLIDER_H
#define PSE_SLIDER_H


class Slider : public QSlider {
Q_OBJECT
public:
    Slider(int interval, Hub *hub, VaccinatieCentrum *centrum, Vaccin* vaccin);

signals:
    void changeText(const QString &str);

public slots:
    void resetSlider();
    void changeValue();
    void changeMaximum();
    void sendVaccins();



private:
    int interval;
    Hub* hub;
    VaccinatieCentrum* centrum;
    Vaccin* vaccin;



};


#endif //PSE_SLIDER_H
