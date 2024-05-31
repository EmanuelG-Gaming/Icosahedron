#ifndef IC_CAMERA_FRUSTRUM_3D_H
#define IC_CAMERA_FRUSTRUM_3D_H


namespace ic {
    /** @brief A camera with a rectangular screen will have a truncated pyramid as the viewable region, 
     *  whose small base represents the actual screen and near clipping plane
     *  while the larger base is the far clipping plane and is usually very distant from the camera's location.
     *  As a convention, each frustrum's plane has a normal that points outside of that solid.  */
    class CameraFrustrum3D {

    };;
}
#endif