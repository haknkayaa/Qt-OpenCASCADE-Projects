//
// Created by sufuk on 1.04.2021.
//

#include "ChartView.h"

ChartView::ChartView() {

}

void ChartView::wheelEvent(QWheelEvent *event) {
    chart()->zoomReset();
    mFactorX *= event->angleDelta().y() > 0 ? 0.5 : 2;
    QRectF rect = chart()->plotArea();
    QPointF c = chart()->plotArea().center();

    rect.setWidth(mFactorX * rect.width());
    rect.setHeight(mFactorX * rect.height());

    rect.moveCenter(c);
    chart()->zoomIn(rect);

    QChartView::wheelEvent(event);
}
