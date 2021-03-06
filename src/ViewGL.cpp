#include "ViewGL.h"
using namespace Win;

ViewGL::ViewGL() : hdc(nullptr), hglrc(nullptr) {}

ViewGL::~ViewGL() {}

void ViewGL::closeContext(HWND handle)
{
    if(!hdc || !hglrc)
        return;

    // delete DC and RC
    ::wglMakeCurrent(nullptr, nullptr);
    ::wglDeleteContext(hglrc);
    ::ReleaseDC(handle, hdc);

    hdc = nullptr;
    hglrc = nullptr;
}

bool ViewGL::createContext(HWND handle, int colorBits, int depthBits, int stencilBits)
{
    // retrieve a handle to a display device context
    hdc = ::GetDC(handle);

    // set pixel format
    if(!setPixelFormat(hdc, colorBits, depthBits, stencilBits))
    {
        ::MessageBox(nullptr, L"Cannot set a suitable pixel format.", L"Error", MB_ICONEXCLAMATION | MB_OK);
        ::ReleaseDC(handle, hdc);                     // remove device context
        return false;
    }

    // create a new OpenGL rendering context
    hglrc = ::wglCreateContext(hdc);
    //::wglMakeCurrent(hdc, hglrc);

    ::ReleaseDC(handle, hdc);
    return true;
}

bool ViewGL::setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
    PIXELFORMATDESCRIPTOR pfd;

    // find out the best matched pixel format
    int pixelFormat = findPixelFormat(hdc, colorBits, depthBits, stencilBits);
    if(pixelFormat == 0)
        return false;

    // set members of PIXELFORMATDESCRIPTOR with given mode ID
    ::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

    // set the fixel format
    if(!::SetPixelFormat(hdc, pixelFormat, &pfd))
        return false;

    return true;
}

int ViewGL::findPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
    int currMode;                               // pixel format mode ID
    int bestMode = 0;                           // return value, best pixel format
    int bestScore = 0;                          // points of best candidate
	int currScore;                              // points of current mode

    PIXELFORMATDESCRIPTOR pfd;

    // search the available formats for the best mode
    for(currMode = 1; ::DescribePixelFormat(hdc, currMode, sizeof(pfd), &pfd) > 0; ++currMode)
    {
        // ignore if cannot support opengl
        if(!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            continue;

        // ignore if cannot render into a window
        if(!(pfd.dwFlags & PFD_DRAW_TO_WINDOW))
            continue;

        // ignore if cannot support rgba mode
        if((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.dwFlags & PFD_NEED_PALETTE))
            continue;

        // ignore if not double buffer
        if(!(pfd.dwFlags & PFD_DOUBLEBUFFER))
            continue;

        // try to find best candidate
        currScore = 0;

        // colour bits
        if(pfd.cColorBits >= colorBits) ++currScore;
        if(pfd.cColorBits == colorBits) ++currScore;

        // depth bits
        if(pfd.cDepthBits >= depthBits) ++currScore;
        if(pfd.cDepthBits == depthBits) ++currScore;

        // stencil bits
        if(pfd.cStencilBits >= stencilBits) ++currScore;
        if(pfd.cStencilBits == stencilBits) ++currScore;

        // alpha bits
        if(pfd.cAlphaBits > 0) ++currScore;

        // check if it is best mode so far
        if(currScore > bestScore)
        {
            bestScore = currScore;
            bestMode = currMode;
        }
    }

    return bestMode;
}

void ViewGL::swapBuffers() const
{
    ::SwapBuffers(hdc);
}

//
//void ViewGL::lButtonDown(WPARAM state, int x, int y)
//{
//	auto poebota = GetPixel(hdc, x, y);
//}
