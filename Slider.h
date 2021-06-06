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
    Slider(int interval, Hub *hub, VaccinatieCentrum *centrum, Vaccin *vaccin);

    /**
     * @return geeft terug of het object correct is geïnitialiseert
     */
    bool isProperlyInitialized() const;

signals:

    /*!
     * verandert de text van het label van de slider
     * @param str niewe waarde van het label
     */
    void changeText(const QString &str);

public slots:

    /*!
     * reset de waarde van de slider naar 0
     * @emit void Slider::changeText(const QString &)
     * \n REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
     */
    void resetSlider();

    /*!
     * verandert de waarde van de slider
     * @emit void Slider::changeText(const QString&)
     * \n REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
     */
    void changeValue();

    /*!
     * verandert het maximum van de slider
     * \n REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
     */
    void changeMaximum();

    /*!
     * stuurt vaccins naar het centrum;
     * \n REQUIRE(isProperlyInitialized(), "Object isn't properly initialized when calling this function!");
     */
    void sendVaccins();


private:
    const Slider *const _initCheck;
    int interval;
    Hub *hub;
    VaccinatieCentrum *centrum;
    Vaccin *vaccin;


};


#endif //PSE_SLIDER_H
