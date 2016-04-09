///////////////////////////////////////////////////////////////////////////////
// ViewFormGL.cpp
// ==============
// View component of OpenGL dialog window
///////////////////////////////////////////////////////////////////////////////

#include "ViewFormGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;

// default ctor
ViewFormGL::ViewFormGL()
{
}

// default dtor
ViewFormGL::~ViewFormGL()
{
}

// initialize all controls
void ViewFormGL::initControls(HWND handle)
{
    // set all controls
    radioFill.set(handle, IDC_FILL);
    radioWireframe.set(handle, IDC_WIREFRAME);
    radioPoint.set(handle, IDC_POINT);
    trackbarRed.set(handle, IDC_RED);
    trackbarGreen.set(handle, IDC_GREEN);
    trackbarBlue.set(handle, IDC_BLUE);

	trackbarTranslateX.set(handle, IDC_TRANSLATEX);
	trackbarTranslateY.set(handle, IDC_TRANSLATEY);
	trackbarTranslateZ.set(handle, IDC_TRANSLATEZ);

	trackbarScaleX.set(handle, IDC_SCALEX);
	trackbarScaleY.set(handle, IDC_SCALEY);
	trackbarScaleZ.set(handle, IDC_SCALEZ);

	trackbarRotateX.set(handle, IDC_ROTATEX);
	trackbarRotateY.set(handle, IDC_ROTATEY);
	trackbarRotateZ.set(handle, IDC_ROTATEZ);

    // initial state
    radioFill.check();
    trackbarRed.setRange(0, 255);
    trackbarRed.setPos(15);
    trackbarGreen.setRange(0, 255);
    trackbarGreen.setPos(15);
    trackbarBlue.setRange(0, 255);
    trackbarBlue.setPos(15);

	trackbarTranslateX.setRange(0, 255);
	trackbarTranslateX.setPos(127);
	trackbarTranslateY.setRange(0, 255);
	trackbarTranslateY.setPos(127);
	trackbarTranslateZ.setRange(0, 255);
	trackbarTranslateZ.setPos(127);

	trackbarScaleX.setRange(0, 255);
	trackbarScaleX.setPos(127);
	trackbarScaleY.setRange(0, 255);
	trackbarScaleY.setPos(127);
	trackbarScaleZ.setRange(0, 255);
	trackbarScaleZ.setPos(127);

	trackbarRotateX.setRange(0, 255);
	trackbarRotateX.setPos(127);
	trackbarRotateY.setRange(0, 255);
	trackbarRotateY.setPos(127);
	trackbarRotateZ.setRange(0, 255);
	trackbarRotateZ.setPos(127);

}


// update trackbars
void ViewFormGL::updateTrackbars(HWND handle, int position)
{

    if(handle == trackbarRed.getHandle())
    {
        trackbarRed.setPos(position);
    }
    else if(handle == trackbarGreen.getHandle())
    {
        trackbarGreen.setPos(position);
    }
    else if(handle == trackbarBlue.getHandle())
    {
        trackbarBlue.setPos(position);
    }
	else if (handle == trackbarTranslateX.getHandle())
		trackbarTranslateX.setPos(position);
	else if (handle == trackbarTranslateY.getHandle())
		trackbarTranslateY.setPos(position);
	else if (handle == trackbarTranslateZ.getHandle())
		trackbarTranslateZ.setPos(position);

	else if (handle == trackbarScaleX.getHandle())
		trackbarScaleX.setPos(position);
	else if (handle == trackbarScaleY.getHandle())
		trackbarScaleY.setPos(position);
	else if (handle == trackbarScaleZ.getHandle())
		trackbarScaleZ.setPos(position);

	else if (handle == trackbarRotateX.getHandle())
		trackbarRotateX.setPos(position);
	else if (handle == trackbarRotateY.getHandle())
		trackbarRotateY.setPos(position);
	else if (handle == trackbarRotateZ.getHandle())
		trackbarRotateZ.setPos(position);

}
