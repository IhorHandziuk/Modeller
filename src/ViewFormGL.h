#pragma once

#include <windows.h>
#include "Controls.h"

namespace Win
{
    class ViewFormGL
    {
    public:
        ViewFormGL();
        ~ViewFormGL();

        void initControls(HWND handle);         // init all controls

        // update controls on the form
        void updateTrackbars(HWND handle, int position);

    protected:

    private:
        // controls
        Win::RadioButton radioFill;
        Win::RadioButton radioWireframe;
        Win::RadioButton radioPoint;

        Win::Trackbar trackbarRed;
        Win::Trackbar trackbarGreen;
        Win::Trackbar trackbarBlue;

		Win::Trackbar trackbarTranslateX;
		Win::Trackbar trackbarTranslateY;
		Win::Trackbar trackbarTranslateZ;

		Win::Trackbar trackbarScaleX;
		Win::Trackbar trackbarScaleY;
		Win::Trackbar trackbarScaleZ;

		Win::Trackbar trackbarRotateX;
		Win::Trackbar trackbarRotateY;
		Win::Trackbar trackbarRotateZ;
    };
}