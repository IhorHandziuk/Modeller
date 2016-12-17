#include <process.h>                                // for _beginthreadex()
#include "ControllerFormGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;


ControllerFormGL::ControllerFormGL(ModelGL* model, ViewFormGL* view) 
	: model(model), view(view) {}

// handle WM_CREATE
int ControllerFormGL::create()
{
    // initialize all controls
    view->initControls(handle);
    return 0;
}

// handle WM_COMMAND
int ControllerFormGL::command(int id, int command, LPARAM msg)
{
    static auto flag = false;

    switch(id)
    {
    case IDC_FILL:
        if(command == BN_CLICKED)
        {
            model->setDrawMode(0);
        }
        break;

    case IDC_WIREFRAME:
        if(command == BN_CLICKED)
        {
            model->setDrawMode(1);
        }
        break;

    case IDC_POINT:
        if(command == BN_CLICKED)
        {
            model->setDrawMode(2);
        }
        break;
    }

    return 0;
}

// handle horizontal scroll notification
int ControllerFormGL::hScroll(WPARAM wParam, LPARAM lParam)
{
    // check if the message comming from trackbar
    HWND trackbarHandle = (HWND)lParam;
    int position = HIWORD(wParam);              // current tick mark position
	//static int prevPosition = position;

    if(trackbarHandle)
    {
        // get control ID
        int trackbarId = ::GetDlgCtrlID(trackbarHandle);

        switch(LOWORD(wParam))
        {
		case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
			position = (int)::SendMessage(trackbarHandle, TBM_GETPOS, 0, 0);
        case TB_THUMBTRACK:     // user dragged the slider
            view->updateTrackbars(trackbarHandle, position);
			if (trackbarId == IDC_RED)
			{
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->change_color(position / 255.0f, i->colour.y, i->colour.z);
				}
			}
			else if (trackbarId == IDC_GREEN)
			{
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->change_color(i->colour.x, position / 255.0f, i->colour.z);
				}
			}
			else if (trackbarId == IDC_BLUE)
			{
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->change_color(i->colour.x, i->colour.y, position / 255.0f);
				}
			}
			else if (trackbarId == IDC_TRANSLATEX) 
			{
				static int prevTransX = 127; //becouse 256 / 2 - 1
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->move((position - prevTransX) / 8.0f, 0, 0); // 8.0 - sensivity
				}
				prevTransX = position;
			}
			else if (trackbarId == IDC_TRANSLATEY)
			{
				static int prevTransY = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->move(0, (position - prevTransY) / 8.0f, 0);
				}
				prevTransY = position;
			}
			else if (trackbarId == IDC_TRANSLATEZ)
			{
				static int prevTransZ = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->move(0, 0, (position - prevTransZ) / 8.0f);
				}
				prevTransZ = position;
			}

			else if (trackbarId == IDC_SCALEX)
			{
				static int prevScaleX = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->scale(1 + (position - prevScaleX) / 32.0f, 1, 1); // 32.0 - sensivity
				}
				prevScaleX = position;
			}
			else if (trackbarId == IDC_SCALEY)
			{
				static int prevScaleY = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->scale(1, 1 + (position - prevScaleY) / 32.0f, 1); 
				}
				prevScaleY = position;
			}
			else if (trackbarId == IDC_SCALEZ)
			{
				static int prevScaleZ = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->scale(1, 1, 1 + (position - prevScaleZ) / 32.0f); 
				}
				prevScaleZ = position;
			}

			else if (trackbarId == IDC_ROTATEX)
			{
				static int prevRotX = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->rotateX((position - prevRotX)/25.0f); //25.0 - sensivity
				}
				prevRotX = position;
			}
			else if (trackbarId == IDC_ROTATEY)
			{
				static int prevRotY = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->rotateY((position - prevRotY) / 25.0f);
				}
				prevRotY = position;
			}
			else if (trackbarId == IDC_ROTATEZ)
			{
				static int prevRotZ = 127;
				for (auto i : model->shapes)
				{
					if (i->marked)
						i->rotateZ((position - prevRotZ) / 25.0f);
				}
				prevRotZ = position;
			}

            break;

        case TB_THUMBPOSITION:  // by WM_LBUTTONUP
            break;

        case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
            break;

        case TB_LINEDOWN:       // by VK_LEFT, VK_UP
            break;

        case TB_TOP:            // by VK_HOME
            break;

        case TB_BOTTOM:         // by VK_END
            break;

        case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
            break;

        case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
            break;

        }
    }
    return 0;
}

