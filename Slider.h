//
// Created by Niels on 6/06/2021.
//
#include <QSlider>
#include <math.h>

#ifndef PSE_SLIDER_H
#define PSE_SLIDER_H


class Slider : public QSlider {
Q_OBJECT
public:
    Slider(int interval = 1);

signals:
    void changeText(const QString &str);

public slots:
    void resetSlider();
    void changeValue();



private:
    int interval;
    int maxValue;


};


#endif //PSE_SLIDER_H
