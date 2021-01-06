#include "Viewer.h"

Viewer::Viewer()
    : RedVal{0}, BlueVal{0}, GreenVal{0}, SpeedVal{0},
      FrameWidth{40}, interval{60}, offset{5},
      Width{0}, Height{0}
{
    
}

void Viewer::InitCamera()
{
    if(!Capture.open(0))
        Capture.open(0);
}

void Viewer::InitKinect()
{
    Capture.open(CV_CAP_OPENNI);
    Capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ);
    Width = Capture.get(CV_CAP_PROP_FRAME_WIDTH);
    Height = Capture.get(CV_CAP_PROP_FRAME_HEIGHT);
}

void Viewer::StopCamera()
{
    Capture.release();
}

QImage Viewer::GetFrame()
{
    Capture.grab();
    Capture.retrieve(Frame, CV_CAP_OPENNI_BGR_IMAGE);
    CalculateValues();
    cv::flip(Frame, Frame, 1);
    cv::putText(Frame, SetText(1, SpeedVal), cv::Point(abs(Width - (Center.X + 3*interval)), FrameWidth - 10), 1, 1, cv::Scalar(255, 255, 255), 1);
    cv::putText(Frame, SetText(2, RedVal), cv::Point(abs(Width - (Center.X - interval)), FrameWidth - 10), 1, 1, cv::Scalar(255, 255, 255), 1);
    cv::putText(Frame, SetText(3, GreenVal), cv::Point(abs(Width - (Center.X - 2*interval)), FrameWidth - 10), 1, 1, cv::Scalar(255, 255, 255), 1);
    cv::putText(Frame, SetText(4, BlueVal), cv::Point(abs(Width - (Center.X - 3*interval)), FrameWidth - 10), 1, 1, cv::Scalar(255, 255, 255), 1);

    cv::line(Frame, cv::Point(abs(Width - (Center.X - interval)), FrameWidth), cv::Point(abs(Width - (Center.X - interval)), Center.Y), cv::Scalar(66, 66, 244), 2, 8, 0);  //  Czerwona
    cv::line(Frame, cv::Point(abs(Width - (Center.X - 2*interval)), FrameWidth), cv::Point(abs(Width - (Center.X - 2*interval)), Center.Y), cv::Scalar(66, 244, 66), 2, 8, 0); //  Zielona
    cv::line(Frame, cv::Point(abs(Width - (Center.X - 3*interval)), FrameWidth), cv::Point(abs(Width - (Center.X - 3*interval)), Center.Y), cv::Scalar(244, 66, 66), 2, 8, 0); //  Niebieska
    cv::line(Frame, cv::Point(abs(Width - (Center.X + 3*interval)), FrameWidth), cv::Point(abs(Width - (Center.X + 3*interval)), Center.Y), cv::Scalar(255, 255, 255), 5, 8, 0); //  Silnik

    cv::circle(Frame, cv::Point(abs(Width - Left.X), Left.Y), 4, cv::Scalar(66, 66, 244), -1, 8, 0); // Kolo na lewej dloni
    cv::circle(Frame, cv::Point(abs(Width - Right.X), Right.Y), 4, cv::Scalar(66, 244, 66), -1, 8, 0); // I na prawej
    cv::circle(Frame, cv::Point(abs(Width - Center.X), Center.Y), 7, cv::Scalar(132, 421 , 451), -1, 8, 0); // Centrum
    QImage Output((const unsigned char *)Frame.data, Frame.cols, Frame.rows, QImage::Format_RGB888);

    return Output;
}

void Viewer::SetValues(const XnVector3D &LeftReceived, const XnVector3D &RightReceived, const XnVector3D &CenterReceived)
{
    Left = LeftReceived;
    Right = RightReceived;
    Center = CenterReceived;
}

uint8_t *Viewer::GetDataFrame()
{
    DataToSend[0] = static_cast<uint8_t>(SpeedVal);
    DataToSend[1] = static_cast<uint8_t>(RedVal);
    DataToSend[2] = static_cast<uint8_t>(GreenVal);
    DataToSend[3] = static_cast<uint8_t>(BlueVal);

    return DataToSend.data();
}

std::string Viewer::SetText(int instance, int val)
{
    std::string TextToDisplay;

    switch (instance)
    {
    case 1: TextToDisplay = "MOT: " + std::to_string(val) + "%";
        break;
    case 2: TextToDisplay = "R: " + std::to_string(val) + "%";
        break;
    case 3: TextToDisplay = "G: " + std::to_string(val) + "%";
        break;
    case 4: TextToDisplay = "B: " + std::to_string(val) + "%";
        break;
    default: TextToDisplay = "...";
        break;
    }

    return TextToDisplay;
}

void Viewer::CalculateValues()
{                                                                                                // Jesli wyzej niz centrum i nizej niz szerokosc ramki
    if(((Right.X < Center.X - interval + offset) && (Right.X > Center.X - interval - offset)) && ((Right.Y <= Center.Y ) && (Right.Y >= FrameWidth)))
    {
        if((Right.Y <= Center.Y) && (Right.Y >= Center.Y - offset)) RedVal = 0;
        else if((Right.Y >= FrameWidth) && (Right.Y <= FrameWidth + offset)) RedVal = 100;
        else RedVal = 100 - ((Right.Y * 100 / (Center.Y - FrameWidth - offset)) - FrameWidth / 2 - offset);
    }
    else RedVal = RedVal;

    if(((Right.X < Center.X - 2*interval + offset) && (Right.X > Center.X - 2*interval - offset)) && ((Right.Y <= Center.Y) && (Right.Y >= FrameWidth)))
    {
        if((Right.Y <= Center.Y) && (Right.Y >= Center.Y - offset)) GreenVal = 0;
        else if((Right.Y >= FrameWidth) && (Right.Y <= FrameWidth + offset)) GreenVal = 100;
        else GreenVal = 100 - ((Right.Y * 100 / (Center.Y - FrameWidth - offset)) - FrameWidth / 2 - offset);
    }
    else GreenVal = GreenVal;

    if(((Right.X < Center.X - 3*interval + offset) && (Right.X > Center.X - 3*interval - offset)) && ((Right.Y <= Center.Y) && (Right.Y >= FrameWidth)))
    {
        if((Right.Y <= Center.Y) && (Right.Y >= Center.Y - offset)) BlueVal = 0;
        else if((Right.Y >= FrameWidth) && (Right.Y <= FrameWidth + offset)) BlueVal = 100;
        else BlueVal = 100 - ((Right.Y * 100 / (Center.Y - FrameWidth - offset)) - FrameWidth / 2 - offset);
    }
    else BlueVal = BlueVal;

    if(((Left.X < Center.X + 3*interval + offset) && (Left.X > Center.X + 3*interval - offset)) && ((Left.Y <= Center.Y) && (Left.Y >= FrameWidth)))
    {
        if((Left.Y <= Center.Y) && (Left.Y >= Center.Y - offset)) SpeedVal = 0;
        else if((Left.Y >= FrameWidth) && (Left.Y <= FrameWidth + offset)) SpeedVal = 100;
        else SpeedVal = 100 - ((Left.Y / (Center.Y - FrameWidth - offset) * 100 ) - FrameWidth / 2 - offset);
    }
    else SpeedVal = SpeedVal;
}


Viewer::~Viewer()
{
    delete [] DataToSend;
}
