//
// Created by hakan on 10.05.2022.
//

#include "QwtSpectrogramPlot.h"

#include <utility>
#include <QObject>
#include <iostream>
#include <qwt_point_data.h>


QwtSpectrogramPlot::QwtSpectrogramPlot(QString dataFilePath) {
    qDebug() << "QwtSpectrogramPlot::QwtSpectrogramPlot starting...";

    m_dataFilePath = std::move(dataFilePath);

    readDataFile(m_dataFilePath);
}

/// Reads the data file and stores the data in m_data.
/// \param dataFilePath
void QwtSpectrogramPlot::readDataFile(QString dataFilePath) {
    // data çekme

    QFile file(m_dataFilePath);
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
        x_AllData.append(list.at(0).toDouble());
        y_AllData.append(list.at(1).toDouble());
        z_AllData.append(list.at(2).toDouble());
        raw_allData.append(list.at(3).toDouble());
    }
    file.close();

    double dataMax = *std::max_element(raw_allData.begin(), raw_allData.end());
    double dataMin = *std::min_element(raw_allData.begin(), raw_allData.end());
    xBinningMax = *std::max_element(x_AllData.begin(), x_AllData.end());
    yBinningMax = *std::max_element(y_AllData.begin(), y_AllData.end());
    zBinningMax = *std::max_element(z_AllData.begin(), z_AllData.end());
    xBinningMax += 1;
    yBinningMax += 1;
    zBinningMax += 1;

    qDebug() << "Binning max: " << xBinningMax << yBinningMax << zBinningMax;

    double *ar_totalXPlane = new double[int(yBinningMax * zBinningMax * 2)]{0};
    double *ar_totalYPlane = new double[int(xBinningMax * zBinningMax * 2)]{0};
    double *ar_totalZPlane = new double[int(yBinningMax * xBinningMax * 2)]{0};


    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file";
        return;
    }
    QTextStream in2(&file);
    while (!in2.atEnd()) {
        QString line = in2.readLine();
        if (line.contains("#")) {
            continue;
        } else {
            QStringList list = line.split(",");

            ar_totalXPlane[int(list.at(2).toDouble() * yBinningMax + list.at(1).toDouble())] += list[3].toDouble();
            ar_totalYPlane[int(list.at(0).toDouble() * zBinningMax + list.at(2).toDouble())] += list[3].toDouble();
            ar_totalZPlane[int(list.at(1).toDouble() * xBinningMax + list.at(0).toDouble())] += list[3].toDouble();
        }
    }
    file.close();


    for (int i = 0; i < int(yBinningMax * zBinningMax); i++) {
        totalXPlane.append(ar_totalXPlane[i]);
    }
    for (int i = 0; i < int(xBinningMax * zBinningMax); i++) {
        totalYPlane.append(ar_totalYPlane[i]);
    }
    for (int i = 0; i < int(yBinningMax * xBinningMax); i++) {
        totalZPlane.append(ar_totalZPlane[i]);
    }
    min_totalXPlane = *std::min_element(totalXPlane.begin(), totalXPlane.end());
    max_totalXPlane = *std::max_element(totalXPlane.begin(), totalXPlane.end());

    min_totalYPlane = *std::min_element(totalYPlane.begin(), totalYPlane.end());
    max_totalYPlane = *std::max_element(totalYPlane.begin(), totalYPlane.end());

    min_totalZPlane = *std::min_element(totalZPlane.begin(), totalZPlane.end());
    max_totalZPlane = *std::max_element(totalZPlane.begin(), totalZPlane.end());

    qDebug() << min_totalXPlane << max_totalXPlane;
    qDebug() << min_totalYPlane << max_totalYPlane;
    qDebug() << min_totalZPlane << max_totalZPlane;
    //  son - data çekme
}

/// Sets the data to be plotted for the spectrogram.
QWidget *QwtSpectrogramPlot::getAxisPlot(Axis axis) {
    QwtPlot *plot = new QwtPlot();
//    plot->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QwtMatrixRasterData *qwtm = new QwtMatrixRasterData();

    if (axis == X_AXIS) {
        qwtm->setInterval(Qt::XAxis, QwtInterval(0, yBinningMax));
        qwtm->setInterval(Qt::YAxis, QwtInterval(0, zBinningMax));
        qwtm->setInterval(Qt::ZAxis, QwtInterval(min_totalXPlane, max_totalXPlane));
        qwtm->setValueMatrix(totalXPlane, yBinningMax);
        plot->setTitle("Total X Plane");
        plot->setAxisTitle(QwtPlot::xBottom, "Y");
        plot->setAxisTitle(QwtPlot::yLeft, "Z");
    } else if (axis == Y_AXIS) {
        qwtm->setInterval(Qt::XAxis, QwtInterval(0, zBinningMax));
        qwtm->setInterval(Qt::YAxis, QwtInterval(0, xBinningMax));
        qwtm->setInterval(Qt::ZAxis, QwtInterval(min_totalYPlane, max_totalYPlane));
        qwtm->setValueMatrix(totalYPlane, zBinningMax);
        plot->setTitle("Total Y Plane");
        plot->setAxisTitle(QwtPlot::xBottom, "Z");
        plot->setAxisTitle(QwtPlot::yLeft, "X");
    } else if (axis == Z_AXIS) {
        qwtm->setInterval(Qt::XAxis, QwtInterval(0, xBinningMax));
        qwtm->setInterval(Qt::YAxis, QwtInterval(0, yBinningMax));
        qwtm->setInterval(Qt::ZAxis, QwtInterval(min_totalZPlane, max_totalZPlane));
        qwtm->setValueMatrix(totalZPlane, xBinningMax);
        plot->setTitle("Total Z Plane");
        plot->setAxisTitle(QwtPlot::xBottom, "X");
        plot->setAxisTitle(QwtPlot::yLeft, "Y");
    }
    qwtm->setResampleMode(qwtm->BilinearInterpolation);


    auto d_spectrogram = new QwtPlotSpectrogram();
    d_spectrogram->setData(qwtm);
    d_spectrogram->setRenderThreadCount(0);
    d_spectrogram->setCachePolicy(QwtPlotRasterItem::PaintCache);

    QList<double> contourLevels;
    if (axis == X_AXIS) {
        float step = max_totalXPlane - min_totalXPlane / 5.0;
        for (int i = 0; i < 5; i++) {
            contourLevels.append(min_totalXPlane + (i * step));
        }
    } else if (axis == Y_AXIS) {
        float step = max_totalYPlane - min_totalYPlane / 5.0;
        for (int i = 0; i < 5; i++) {
            contourLevels.append(min_totalYPlane + (i * step));
        }
    } else if (axis == Z_AXIS) {
        float step = max_totalZPlane - min_totalZPlane / 5.0;
        for (int i = 0; i < 5; i++) {
            contourLevels.append(min_totalZPlane + (i * step));
        }
    }

    d_spectrogram->setContourLevels(contourLevels);

    // color map
    auto *colormap = new QwtLinearColorMap(Qt::white, Qt::black);
    colormap->addColorStop(0.00, QColor(253, 253, 253));
    colormap->addColorStop(0.10, QColor(188, 80, 248));
    colormap->addColorStop(0.27, QColor(255, 27, 0));
    colormap->addColorStop(0.40, QColor(255, 224, 0));
    colormap->addColorStop(0.55, QColor(138, 255, 17));
    colormap->addColorStop(0.70, QColor(0, 251, 97));
    colormap->addColorStop(0.90, QColor(0, 127, 255));
    colormap->addColorStop(10.0, QColor(0, 8, 117));
    d_spectrogram->setColorMap(colormap);

    QwtScaleWidget *rightAxis = plot->axisWidget(QwtPlot::yRight);
    const QwtInterval zInterval = d_spectrogram->data()->interval(Qt::ZAxis);
    rightAxis->setTitle("Value");
    rightAxis->setColorBarEnabled(true);
    rightAxis->setColorMap(zInterval, colormap);

    plot->setAxisScale(QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue());
    plot->enableAxis(QwtPlot::yRight);

    d_spectrogram->attach(plot);

    auto widget = new QWidget();
//    widget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    auto *buttonLayout = new QHBoxLayout;
    auto savebutton = new QPushButton("Save");
    QObject::connect(savebutton, &QPushButton::clicked, [this, plot, d_spectrogram]() {
        qDebug() << "Starting... ";

        plot->enableAxis(QwtPlot::yRight, false);
        plot->enableAxis(QwtPlot::xBottom, false);
        plot->enableAxis(QwtPlot::yLeft, false);
        QwtPlotRenderer renderer;
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, true);
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground, true);
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasFrame, true);
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardTitle, true);
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardLegend, true);
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardFooter, true);
        renderer.setLayoutFlag(QwtPlotRenderer::FrameWithScales, false);
        renderer.renderDocument(plot, "deneme4.png", QSizeF(500, 500), 85);
        plot->enableAxis(QwtPlot::yRight, true);
        plot->enableAxis(QwtPlot::xBottom, true);
        plot->enableAxis(QwtPlot::yLeft, true);

        qDebug() << "Saved. ";
    });

    auto enableContour = new QPushButton("Contour");
    enableContour->setCheckable(true);
    QObject::connect(enableContour, &QPushButton::clicked, [this, enableContour, d_spectrogram, plot]() {
        if (enableContour->isChecked()) {
            qDebug() << "contour enabled";
            d_spectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, true);
            plot->replot();
        } else {
            qDebug() << "contour disabled";
            d_spectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, false);
            plot->replot();
        }
    });

    // new spacer
    auto spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    buttonLayout->addSpacerItem(spacer);
    buttonLayout->addWidget(enableContour);
    buttonLayout->addWidget(savebutton);
    widget->setLayout(buttonLayout);

    QWidget *container = new QWidget();
    container->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    auto *layout = new QVBoxLayout;
    layout->addWidget(widget);
    layout->addWidget(plot);
    container->setLayout(layout);

    qDebug() << "IMAGE SAvıng... ";
    if(axis == X_AXIS){
        exportImage(plot, "output-x.png");
    }
    else if(axis == Y_AXIS){
        exportImage(plot, "output-y.png");
    }
    else if(axis == Z_AXIS){
        exportImage(plot, "output-z.png");
    }

    return container;
}


void QwtSpectrogramPlot::exportImage(QwtPlot* plot, QString fileName){
    plot->enableAxis(QwtPlot::yRight, false);
    plot->enableAxis(QwtPlot::xBottom, false);
    plot->enableAxis(QwtPlot::yLeft, false);
    QwtPlotRenderer renderer;
    plot->replot();
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasFrame, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardTitle, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardLegend, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardFooter, true);
    renderer.setLayoutFlag(QwtPlotRenderer::FrameWithScales, false);
    renderer.renderDocument(plot, fileName, QSizeF(500, 500), 85);
    plot->enableAxis(QwtPlot::yRight, true);
    plot->enableAxis(QwtPlot::xBottom, true);
    plot->enableAxis(QwtPlot::yLeft, true);

    qDebug() << "Saved. ";
}
