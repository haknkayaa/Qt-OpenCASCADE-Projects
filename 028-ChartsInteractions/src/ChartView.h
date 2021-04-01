//
// Created by sufuk on 1.04.2021.
//

#ifndef OPENCASCADEPROJECT_CHARTVIEW_H
#define OPENCASCADEPROJECT_CHARTVIEW_H

#include <QtWidgets>
#include <QtCharts>

class ChartView : public QChartView{
Q_OBJECT

qreal mFactorX=1.0;
qreal mFactorY=1.0;

public:
    ChartView();

protected:
    void wheelEvent(QWheelEvent *event) override;
};


#endif //OPENCASCADEPROJECT_CHARTVIEW_H
