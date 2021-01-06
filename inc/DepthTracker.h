#ifndef DEPTHTRACKER_H
#define DEPTHTRACKER_H
#include <XnCppWrapper.h>

class DepthTracker
{
public:
    DepthTracker(xn::Context& context, xn::ScriptNode& script);
    ~DepthTracker();

    XnStatus Init();
    XnStatus Run();
    XnBool CheckFile(const char *fn);
    XnStatus StopTracking();
    XnVector3D GetLeftCoordonates();
    XnVector3D GetRightCoordonates();
    XnVector3D GetCenterCoordonates();

private:
    xn::Context Context;
    xn::ScriptNode ScriptNode;
    xn::UserGenerator UserGenerator;
    xn::DepthGenerator Depth;
    XnPoint3D Left, Right, Center;
    XnSkeletonJointPosition LeftHand, RightHand, Torso;
    static constexpr int USERS{1};
    XnBool NeedPose;
    XnChar PoseName[1]{""};
    XnUserID *aUsers;
    XnUInt16 nUsers;

    static void XN_CALLBACK_TYPE NewUser(xn::UserGenerator&, XnUserID nId, void* pCookie);
    static void XN_CALLBACK_TYPE LostUser(xn::UserGenerator&, XnUserID nId, void*);
    static void XN_CALLBACK_TYPE PoseDetected(xn::PoseDetectionCapability&, const XnChar* strPose, XnUserID nId, void* pCookie);
    static void XN_CALLBACK_TYPE CalibrationStart(xn::SkeletonCapability&, XnUserID nId, void*);
    static void XN_CALLBACK_TYPE CalibrationComplete(xn::SkeletonCapability&, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);
};

#endif // DEPTHTRACKER_H
