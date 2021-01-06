#include "Mainwindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Viewer(),
      ui(new Ui::MainWindow),
      nRetVal{XN_STATUS_OK}
{
    ui->setupUi(this);
    strcpy(fn, XML_PATH);
    Tracker = new DepthTracker(g_Context, g_scriptNode);
    nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);

    T1 = new QTimer;
    connect(T1, &QTimer::timeout, this, &MainWindow::ReceiveImage);
    connect(T1, &QTimer::timeout, this, &MainWindow::ReceiveCoordinates);
}

void MainWindow::ReceiveImage()
{
    ui->Camera1->setPixmap(QPixmap::fromImage(GetFrame().rgbSwapped()).scaled((ui->Camera1->size()), Qt::KeepAspectRatio));
}

void MainWindow::ReceiveCoordinates()
{
    Tracker->Run();
    Viewer::SetValues(Tracker->GetLeftCoordonates(), Tracker->GetRightCoordonates(), Tracker->GetCenterCoordonates());
    serial.Send(Viewer::GetDataFrame());
}

void MainWindow::on_StartButton_clicked()
{
    T1->start(30);
    Viewer::InitKinect();
    Tracker->Init();
//    InitCamera();
    serial.ConfiguratePort(PORTNAME);
}

void MainWindow::on_QuitButton_clicked()
{
    qApp->quit();
}

void MainWindow::on_StopButton_clicked()
{
    Viewer::StopCamera();
}

MainWindow::~MainWindow()
{
    delete Tracker;
    delete T1;
}
