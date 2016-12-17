#pragma once

#include "Controller.h"
#include "ModelGL.h"

namespace Win
{
    class ControllerMain :public Controller
    {
    public:
        ControllerMain(ModelGL* );
        ~ControllerMain() {};

        void setGLHandle(HWND handle)               { glHandle = handle; }
        void setFormHandle(HWND handle)             { formHandle = handle; }

        int command(int id, int cmd, LPARAM msg)     override;   // for WM_COMMAND
        int close()                                  override;
        int create()                                 override;   // for WM_CRERATE
        int destroy()                                override;
        int size(int w, int h, WPARAM wParam)        override;   // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

    private:
        HWND glHandle;                              // handle of gl window
        HWND formHandle;                            // handle of form dialog
		ModelGL* model;                             // pointer to model component

    };
}
