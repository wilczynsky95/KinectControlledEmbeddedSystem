#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QLabel>
#include <QGraphicsItem>
#include <opencv2/imgproc/imgproc.hpp>
#include <XnCppWrapper.h>
#include "Viewer.h"
#include "DepthTracker.h"
#include "SerialPort.h"
#define XML_PATH "../SamplesConfig.xml"
#define PORTNAME "/dev/ttyUSB0"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Viewer
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_QuitButton_clicked();
    void on_StartButton_clicked();
    void on_StopButton_clicked();

private:
    SerialPort serial;
    Ui::MainWindow *ui;
    QTimer *T1;
    xn::Context g_Context;
    xn::ScriptNode g_scriptNode;
    DepthTracker *Tracker;
    XnStatus nRetVal;
    xn::EnumerationErrors errors;
    string fn;

    void ReceiveImage();
    void ReceiveCoordinates();
};

#endif // MAINWINDOW_H
