//
// Created by hakan on 10.05.2022.
//

#include "QwtSpectrogramPlot.h"

QwtSpectrogramPlot::QwtSpectrogramPlot (QWidget *parent) :  QWidget(parent) {
    qDebug() << "QwtSpectrogramPlot::QwtSpectrogramPlot";


    // data çekme
    QVector<double> totalXPlane(2500);
    QVector<double> totalYPlane(2500);
    QVector<double> totalZPlane(2500);

    double x[50] = {0}, y[50] = {0}, z[50] = {0};
    QVector<double> allX, allY, allZ, allData;
    QVector<double> xData, yData, zData;

    QString dataFileName = "../3d_data.txt";
    QFile file(dataFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains("#"))
            continue;

        QStringList list = line.split(",");
        allX.append(list.at(0).toDouble());
        allY.append(list.at(1).toDouble());
        allZ.append(list.at(2).toDouble());
        allData.append(list.at(3).toDouble());

        x[list.at(0).toInt()] += list[3].toDouble();
        y[list.at(1).toInt()] += list[3].toDouble();
        z[list.at(2).toInt()] += list[3].toDouble();

        totalXPlane[list.at(1).toDouble() * 50 + list.at(0).toDouble()] += list[3].toDouble();
        totalYPlane[list.at(2).toDouble() * 50 + list.at(1).toDouble()] += list[3].toDouble();
        totalZPlane[list.at(0).toDouble() * 50 + list.at(2).toDouble()] += list[3].toDouble();
    }
    file.close();

    double min_totalXPlane = *std::min_element(totalXPlane.begin(), totalXPlane.end());
    double max_totalXPlane = *std::max_element(totalXPlane.begin(), totalXPlane.end());

    double min_totalYPlane = *std::min_element(totalYPlane.begin(), totalYPlane.end());
    double max_totalYPlane = *std::max_element(totalYPlane.begin(), totalYPlane.end());

    double min_totalZPlane = *std::min_element(totalZPlane.begin(), totalZPlane.end());
    double max_totalZPlane = *std::max_element(totalZPlane.begin(), totalZPlane.end());

    for (int i = 0; i < 50; i++) {
        qDebug() << x[i] << " " << y[i] << " " << z[i];
    }

    double dataMax = *std::max_element(allData.begin(), allData.end());
    double dataMin = *std::min_element(allData.begin(), allData.end());
    double xBinningMax = *std::max_element(allX.begin(), allX.end());
    double yBinningMax = *std::max_element(allY.begin(), allY.end());
    double zBinningMax = *std::max_element(allZ.begin(), allZ.end());

    for (int i = 0; i < 50; i++) {
        xData.append(x[i]);
    }

    double xMin = *std::min_element(xData.begin(), xData.end());
    double xMax = *std::max_element(xData.begin(), xData.end());

    for (int i = 0; i < 50; i++) {
        yData.append(y[i]);
    }

    double yMin = *std::min_element(yData.begin(), yData.end());
    double yMax = *std::max_element(yData.begin(), yData.end());

    for (int i = 0; i < 50; i++) {
        zData.append(z[i]);
    }

    double zMin = *std::min_element(zData.begin(), zData.end());
    double zMax = *std::max_element(zData.begin(), zData.end());

    QwtMatrixRasterData *qwtm = new QwtMatrixRasterData();
    qwtm->setInterval(Qt::XAxis, QwtInterval(0, 50));
    qwtm->setInterval(Qt::YAxis, QwtInterval(0, 50));

    qwtm->setInterval(Qt::ZAxis, QwtInterval(min_totalXPlane, max_totalXPlane));
    qwtm->setValueMatrix(totalXPlane, 50);
//    qwtm->setInterval(Qt::ZAxis, QwtInterval(min_totalYPlane, max_totalYPlane));
//    qwtm->setValueMatrix(totalYPlane, 50);
//    qwtm->setInterval(Qt::ZAxis, QwtInterval(min_totalZPlane, max_totalZPlane));
//    qwtm->setValueMatrix(totalZPlane, 50);

    qwtm->setResampleMode(qwtm->BilinearInterpolation);

    //  son - data çekme

    auto plot = new QwtPlot(this);

    auto d_spectrogram = new QwtPlotSpectrogram();
    d_spectrogram->setData(qwtm);
    d_spectrogram->setRenderThreadCount(0);
    d_spectrogram->setCachePolicy(QwtPlotRasterItem::PaintCache);

    QList<double> contourLevels;
    contourLevels << 0.1  << 0.3 << 0.5 << 0.7 << 0.9;
    d_spectrogram->setContourLevels(contourLevels);
    d_spectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, true);

    // color map
    QwtLinearColorMap *colormap = new QwtLinearColorMap(Qt::white, Qt::black);
    colormap->addColorStop(0.0, QColor(253, 253, 253));
    colormap->addColorStop(0.1, QColor(188, 80, 248));
    colormap->addColorStop(0.275, QColor(255, 27, 0));
    colormap->addColorStop(0.4, QColor(255, 224, 0));
    colormap->addColorStop(0.55, QColor(138, 255, 17));
    colormap->addColorStop(0.7, QColor(0, 251, 97));
    colormap->addColorStop(0.9, QColor(0, 127, 255));
    colormap->addColorStop(1, QColor(0, 8, 117));
    d_spectrogram->setColorMap(colormap);



    QwtScaleWidget *rightAxis = plot->axisWidget(QwtPlot::yRight);
    const QwtInterval zInterval = d_spectrogram->data()->interval(Qt::ZAxis);
    rightAxis->setTitle("Value");
    rightAxis->setColorBarEnabled(true);
    rightAxis->setColorMap(zInterval, colormap);

    plot->setAxisScale(QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue());
    plot->enableAxis(QwtPlot::yRight);

    d_spectrogram->attach(plot);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(plot);
    setLayout(layout);
}