#ifndef VIEWER_H
#define VIEWER_H
#include <opencv2/opencv.hpp>
#include <QImage>
#include <XnCppWrapper.h>
#include <string.h>
#include <QDebug>
#include <array>

class Viewer
{
public:
    Viewer();
    ~Viewer();

    QImage GetFrame();
    void InitCamera();
    void InitKinect();
    void StopCamera();
    void SetValues(const XnVector3D&, const XnVector3D&, const XnVector3D&);
    uint8_t *GetDataFrame();

private:
    XnVector3D Left;
    XnVector3D Right;
    XnVector3D Center;
    cv::VideoCapture Capture;
    cv::Mat Frame;
    int RedVal;
    int BlueVal;
    int GreenVal;
    int SpeedVal;
    const int FrameWidth;
    const int interval;
    const int offset;
    int Width, Height;
    std::array<uint8_t, 4> DataToSend;

    void CalculateValues();
    std::string SetText(int Instance, int Val);
};

#endif // VIEWER_H
