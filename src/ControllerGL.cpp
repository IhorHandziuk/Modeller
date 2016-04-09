///////////////////////////////////////////////////////////////////////////////
// ControllerGL.cpp
// ================
// Derived Controller class for OpenGL window
// It is the controller of OpenGL rendering window. It initializes DC and RC,
// when WM_CREATE called, then, start new thread for OpenGL rendering loop.
//
// When this class is constructed, it gets the pointers to model and view
// components.
///////////////////////////////////////////////////////////////////////////////

#include <process.h>                                // for _beginthreadex()
#include "ControllerGL.h"
#include "Log.h"
#include <iostream>
using namespace Win;

HANDLE console = NULL; // консоль для вывода 

					   // создаем консоль 
void CreateConsole(HWND hwnd)
{
	if (!AllocConsole())
		MessageBox(hwnd, L"Can't create console!", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
	else
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		freopen("CONOUT$", "wt", stdout);
		printf("First string\n\n");
	}

}

// default contructor
ControllerGL::ControllerGL(ModelGL* model, ViewGL* view) : model(model), view(view),
														   threadHandle(0), threadId(0),
														   loopFlag(false) 
{

}

// handle WM_CLOSE
int ControllerGL::close()
{
	loopFlag = false;
	::WaitForSingleObject(threadHandle, INFINITE);   // wait for rendering thread is terminated

	// close OpenGL Rendering Context (RC)
	view->closeContext(handle);

	::DestroyWindow(handle);
	Win::log("OpenGL window is destroyed.");
	return 0;
}

// handle WM_CREATE
int ControllerGL::create()
{
	//CreateConsole(handle);
	// create a OpenGL rendering context
	if(!view->createContext(handle, 32, 24, 8))
	{
		Win::log(L"[ERROR] Failed to create OpenGL rendering context from ControllerGL::create().");
		return -1;
	}

	// create a thread for OpenGL rendering
	// The params of _beginthreadex() are security, stackSize, functionPtr, argPtr, initFlag, threadId.
	threadHandle = (HANDLE)_beginthreadex(0, 0, (unsigned (__stdcall *)(void *))threadFunction, this, 0, &threadId);
	if(threadHandle)
	{
		loopFlag = true;
		Win::log(L"Created a rendering thread for OpenGL.");
	}
	else
	{
		Win::log(L"[ERROR] Failed to create rendering thread from ControllerGL::create().");
	}

	return 0;
}

// handle WM_PAINT
int ControllerGL::paint()
{
	return 0;
}

// handle WM_COMMAND
int ControllerGL::command(int id, int cmd, LPARAM msg)
{
	return 0;
}

// route to worker thread
// The function prototype is:
// unsigned int (__stdcall *)(void *)
void ControllerGL::threadFunction(void* param)
{
	((ControllerGL*)param)->runThread();
}

// rendering thread
// initialize OpenGL states and start rendering loop
void ControllerGL::runThread()
{
	// set the current RC in this thread
	::wglMakeCurrent(view->getDC(), view->getRC());

	// initialize OpenGL states
	model->init();
	Win::log(L"Initialized OpenGL states.");

	// cofigure projection matrix
	RECT rect;
	::GetClientRect(handle, &rect);
	model->resizeWindow(rect.right, rect.bottom);
	Win::log(L"Initialized OpenGL viewport and projection matrix.");

	// rendering loop
	Win::log(L"Entering OpenGL rendering thread...");
	while(loopFlag)
	{
		Sleep(4);                       // yield to other processes or threads
		model->draw();
		view->swapBuffers();
	}

	// terminate rendering thread
	::wglMakeCurrent(0, 0);             // unset RC
	::CloseHandle(threadHandle);
	Win::log(L"Exit OpenGL rendering thread.");
}

// handle Left mouse down
int ControllerGL::lButtonDown(WPARAM state, int x, int y)
{
	// update mouse position
	model->setMouseLeft(true);
	model->setMousePosition(x, y);
	model->setPicking(true);
	return 0;
}

// handle Left mouse up
int ControllerGL::lButtonUp(WPARAM state, int x, int y)
{
	// update mouse position
	model->setMousePosition(x, y);

	model->setMouseLeft(false);

	return 0;
}

// handle reft mouse down
int ControllerGL::rButtonDown(WPARAM state, int x, int y)
{
	// update mouse position
	model->setMousePosition(x, y);

	if(state == MK_RBUTTON)
	{
		model->setMouseRight(true);
	}

	return 0;
}

// handle reft mouse up
int ControllerGL::rButtonUp(WPARAM state, int x, int y)
{
	// update mouse position
	model->setMousePosition(x, y);

	model->setMouseRight(false);

	return 0;
}

// handle WM_MOUSEMOVE
int ControllerGL::mouseMove(WPARAM state, int x, int y)
{
	static int prevX = x;
	static int prevY = y;
	const float pi = 3.1415;
	switch (state)
	{
	case MK_LBUTTON:
		model->rotateCamera(x, y);
		break;
	case MK_RBUTTON:

#pragma region usefull comments
		//set XOY
		//model->setCameraAngle(0, 0);
		//set XOZ
		//model->setCameraAngle(-90, 0);
		//set YOZ
		//model->setCameraAngle(-90, -90);

		//float dx = (x - prevX) / 10.0f; //case XY
		//float dy = (prevY - y) / 10.0f;
		//float dz = 0;

		//float dx = (x - prevX) / 10.0f; //case XZ
		//float dy = 0;
		//float dz = (prevY - y) / 10.0f;

		//float dx = 0; //case YZ
		//float dy = (x - prevX) / 10.0f;
		//float dz = (prevY - y) / 10.0f;
#pragma endregion

		float dx = cos(model->cameraAngleY * pi / 180.0f) * (x - prevX) / 10.f;
		float dy = cos(model->cameraAngleX * pi / 180) * (prevY - y) / 10.f - sin(model->cameraAngleY * pi / 180.0f) * (x - prevX) / 10.f;
		float dz = -sin(model->cameraAngleX * pi / 180.0f) * (prevY - y) / 10.0f;
		for (auto i : model->shapes)
		{
			if (i->marked)
			{
				i->move(dx, dy, dz);
			}
		}
		break;
	}
	prevX = x;
	prevY = y;
	return 0;
}

// handle WM_SIZE
int ControllerGL::size(int w, int h, WPARAM wParam)
{
	model->resizeWindow(w, h);
	Win::log(L"OpenGL window is resized: %dx%d.", w, h);
	return 0;
}

// handle WM_KEYDOWN
int ControllerGL::keyDown(int key, LPARAM lParam)
{
	static bool ctrl = (key == VK_CONTROL);
	switch (key)
	{
	case VK_DELETE:
		for (auto i = model->shapes.begin(); i != model->shapes.end();)
		{
			if ((*i)->marked)
			{
				model->shapes.erase(i);
				i = model->shapes.begin();
			}
			else i++;
		}
		break;
	case '1':
		//set XOY
		model->setCameraAngle(0, 0);
		break;
	case '2':
		//set XOZ
		model->setCameraAngle(-90, 0);
		break;
	case '3':
		//set YOZ
		model->setCameraAngle(-90, -90);
		break;
	}
	if (ctrl)
	{
		model->setControl(true);
		switch (key)
		{	
		case 'A':
			for (auto i : model->shapes)
				i->marked = true;
			break;
		//case 'C':
		//	break;
		//case 'V':
		//	break;
		//case 'X':
		//	break;
		//case 'N':
		//	break;
		//case 'O':
		//	break;
		//case 'S':
		//	break;
		}
	}
	ctrl = (key == VK_CONTROL);
	return 0;
}

// handle WM_KEYUP
int ControllerGL::keyUp(int key, LPARAM lParam)
{
	if (key == VK_CONTROL) model->setControl(false);
	return 0;
}
