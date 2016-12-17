#pragma once

#include "Controller.h"
#include "ViewGL.h"
#include "ModelGL.h"


namespace Win
{
    class ControllerGL : public Controller
    {
    public:
        ControllerGL(ModelGL* model, ViewGL* view);
        ~ControllerGL() {};

        int close()                                    override;   // close the RC and OpenGL window
        int command(int id, int cmd, LPARAM msg)       override;   // for WM_COMMAND
        int create()                                   override;   // create RC for OpenGL window and start new thread for rendering
        int paint()                                    override;
        int size(int w, int h, WPARAM wParam)          override;
		int keyDown(int key, LPARAM lParam)            override;
		int keyUp(int key, LPARAM lParam)              override;
        int lButtonDown(WPARAM state, int x, int y)    override;
        int lButtonUp(WPARAM state, int x, int y)      override;
        int rButtonDown(WPARAM state, int x, int y)    override;
        int rButtonUp(WPARAM state, int x, int y)      override;
        int mouseMove(WPARAM state, int x, int y)      override;


   
    private:
        static void threadFunction(void* arg);      // static thread function, it will route to member function, runThread()
        void runThread();                           // thread for OpenGL rendering

        ModelGL* model;                             
        ViewGL* view;                               
        HANDLE threadHandle;
        unsigned int threadId;
        volatile bool loopFlag;                     // rendering loop flag
    };
}
