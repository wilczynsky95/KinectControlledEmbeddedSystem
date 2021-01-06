#include "DepthTracker.h"
#include <XnCppWrapper.h>
#include <QDebug>

#define INFO(RetVal, what)                                          \
    if (RetVal != XN_STATUS_OK)                                     \
{                                                                   \
    printf("%s Blad: %s\n", what, xnGetStatusString(RetVal));       \
    return RetVal;                                                  \
}

DepthTracker::DepthTracker(xn::Context &context, xn::ScriptNode &script)
    : Context{context},
      ScriptNode{script},
      Left{XnPoint3D{0, 0, 0}},
      Right{XnPoint3D{0, 0, 0}},
      Center{XnPoint3D{0, 0, 0}},
      NeedPose{false},
      aUsers{new XnUserID[USERS]}
{

}

XnStatus DepthTracker::Init()
{
    XnStatus RetVal = XN_STATUS_OK;
    XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;

    RetVal = Context.FindExistingNode(XN_NODE_TYPE_DEPTH, Depth);
    if (RetVal != XN_STATUS_OK)
    {
        RetVal = UserGenerator.Create(Context);
        INFO(RetVal, "Blad generatora glebi");
    }
    RetVal = Context.FindExistingNode(XN_NODE_TYPE_USER, UserGenerator);
    if (RetVal != XN_STATUS_OK)
    {
        RetVal = UserGenerator.Create(Context);
        INFO(RetVal, "Blad generatora uzytkownikow");
    }
    if (!UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        printf("Zastosowany generator nie wspiera szkieletu !\n");
        return 1;
    }

    RetVal = UserGenerator.RegisterUserCallbacks(NewUser, LostUser, this, hUserCallbacks);
    RetVal = UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(CalibrationStart, this, hCalibrationStart);
    RetVal = UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(CalibrationComplete, this, hCalibrationComplete);

    if (UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        NeedPose = TRUE;
        if (!UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            printf("Wykryto postac, ale wezel nie wspiera\n");
            return 1;
        }
        RetVal = UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(PoseDetected, this, hPoseDetected);
        INFO(RetVal, "Register to Pose Detected");
        UserGenerator.GetSkeletonCap().GetCalibrationPose(PoseName);
    }
    UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
    Context.StartGeneratingAll();

    return XN_STATUS_OK;
}

XnStatus DepthTracker::Run()
{
    Context.WaitOneUpdateAll(UserGenerator);
    nUsers = USERS;
    UserGenerator.GetUsers(aUsers, nUsers);

    for(XnUInt16 i = 0; i < nUsers; i++)
    {
        if(UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]) == FALSE) continue;

        UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_LEFT_HAND, LeftHand);
        UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_RIGHT_HAND, RightHand);
        UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_TORSO, Torso);

        Depth.ConvertRealWorldToProjective(1, &LeftHand.position, &Left);
        Depth.ConvertRealWorldToProjective(1, &RightHand.position, &Right);
        Depth.ConvertRealWorldToProjective(1, &Torso.position, &Center);
    }

    return XN_STATUS_OK;
}

XnStatus DepthTracker::StopTracking()
{
    ScriptNode.Release();
    UserGenerator.Release();
    Context.Release();
    Depth.Release();

    return XN_STATUS_OK;
}

XnBool DepthTracker::CheckFile(const char *fn)
{
    XnBool file;
    xnOSDoesFileExist(fn, &file);
    return file;
}

XnVector3D DepthTracker::GetLeftCoordonates()
{
    return Left;
}

XnVector3D DepthTracker::GetRightCoordonates()
{
    return Right;
}

XnVector3D DepthTracker::GetCenterCoordonates()
{
    return Center;
}

DepthTracker::~DepthTracker()
{
    delete [] aUsers;
}

void XN_CALLBACK_TYPE DepthTracker::NewUser(xn::UserGenerator&, XnUserID nId, void *pCookie)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    DepthTracker* pThis = static_cast<DepthTracker*>(pCookie);
    printf("Nowy uzytkownik: %d\n", nId);
    if (pThis->NeedPose)
    {
        pThis->UserGenerator.GetPoseDetectionCap().StartPoseDetection(pThis->PoseName, nId);
    }
    else
    {
        pThis->UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
}

void XN_CALLBACK_TYPE DepthTracker::LostUser(xn::UserGenerator&, XnUserID nId, void*)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("Zgubiono uzytkownika nr %d\n", nId);
}

void XN_CALLBACK_TYPE DepthTracker::PoseDetected(xn::PoseDetectionCapability&, const XnChar *strPose, XnUserID nId, void *pCookie)
{
    DepthTracker* pThis = static_cast<DepthTracker*>(pCookie);
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("Wykryto poze %s u uzytkownika nr %d\n", strPose, nId);
    pThis->UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    pThis->UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void XN_CALLBACK_TYPE DepthTracker::CalibrationStart(xn::SkeletonCapability&, XnUserID nId, void *)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("Rozpoczeto kalibracje dla uzytkownika nr %d\n", nId);
}

void XN_CALLBACK_TYPE DepthTracker::CalibrationComplete(xn::SkeletonCapability&, XnUserID nId, XnCalibrationStatus eStatus, void *pCookie)
{
    DepthTracker* pThis = static_cast<DepthTracker*>(pCookie);
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
    {
        printf("Kalibracja zakonczona, rozpoczynam sledzenie uzytkownika nr %d\n", nId);
        pThis->UserGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        printf("Kalibracja nie powiodla sie dla uzytkownika nr %d\n", nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Reczne zatrzymanie kalibracji !");
            return;
        }
        if (pThis->NeedPose)
        {
            pThis->UserGenerator.GetPoseDetectionCap().StartPoseDetection(pThis->PoseName, nId);
        }
        else
        {
            pThis->UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
}

