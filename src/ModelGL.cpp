// Model component of OpenGL
#if _WIN32
    #include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "ModelGL.h"
#include "Bmp.h"
#include "Shape.h"
#include <iostream>

void ModelGL::init(void)
{
	glClearColor(0.6, 0.6, 0.6, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	
	/****************************************/
	/*       Set up OpenGL lights           */
	/****************************************/
	GLfloat light1_ambient[] = { 0.1f, 0.1f, 0.3f, 1.0f };
	GLfloat light1_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light1_position[] = { 10.0, 10.0, 10.0, 0.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glEnable(GL_COLOR_MATERIAL);
	glDepthRange(0.0, 1.0); /* The default z mapping */

	
	Shape* it = new Cuboid(1, 1, 1);
	it->scale(2, 2, 2);
	it->move(1, 1, 1);
	shapes.push_back(it);
}

// toggle to resize window
void ModelGL::resizeWindow(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	windowResized = true;
}

// default ctor
ModelGL::ModelGL() 
	: windowWidth(0), windowHeight(0), mouseLeftDown(false),
	mouseRightDown(false), changeDrawMode(false), drawMode(0),
	cameraAngleX(0), cameraAngleY(0), cameraDistance(5),
	frameBuffer(0), bufferSize(0), windowResized(false), 
	ctrl(false), pick(false), hideGrid(false), isAxes(true)
	{}


// destructor
ModelGL::~ModelGL()
{
    // deallocate framebuffer
    //delete [] frameBuffer;
    //frameBuffer = 0;
}

// set camera position and lookat direction
void ModelGL::setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}


// configure projection and viewport
void ModelGL::setViewport(int w, int h)
{
    // assign the width/height of viewport
    windowWidth = w;
    windowHeight = h;
    //bufferSize = w * h * 4; // rgba
    //frameBuffer = new unsigned char [bufferSize];

    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // set perspective viewing frustum
    float aspectRatio = (float)w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(w)/h, 0.1f, 80.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
}


// draw 2D/3D scene
void ModelGL::draw()
{
	static float angle = 0;

    if(windowResized)
    {
        setViewport(windowWidth, windowHeight);
		setCamera(0, 0, 15, 0, 0, 0);
        windowResized = false;
    }

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform camera
    glTranslatef(0, 0, cameraDistance);
    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 0, 1);   // heading

    glPushMatrix(); // draw sphere
        glRotatef(angle, 0.39875f, 0.91706f, 0.0f);

	if (!hideGrid)
		axes.draw();

	for (int i = 0; i < shapes.size(); i++)
	{
		if (pick)
		{
			glDisable(GL_LIGHTING);
			glColor3ub(shapes[i]->ID, 0, 0);
		}
		else glColor3f(shapes[i]->colour.x, shapes[i]->colour.y, shapes[i]->colour.z);
		shapes[i]->draw();
	}
	glPopMatrix();
	glPopMatrix();

	if (pick) {
		bool clear = true;;
		if (!ctrl) {
			for (auto i : shapes) i->marked = false;
		}
		unsigned char pixel[4];
		glReadPixels(mouseX, windowHeight - mouseY - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
		for (auto i : shapes)
		{
			if (i->ID == (int)pixel[0])
			{
				i->marked = true;
				clear = false;
			}
		}
		pick = false;
	}

    if(changeDrawMode)
    {
        if(drawMode == 0)           // fill mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if(drawMode == 1)      // wireframe mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else if(drawMode == 2)      // point mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
    }
	glEnable(GL_LIGHTING);
}

// load a BMP as texture
unsigned int ModelGL::loadTextureBmp(const char* fileName)
{
    int chans, x, y;
    void* buf;
    Image::Bmp bmp;

    bmp.read(fileName);
    x = bmp.getWidth();
    y = bmp.getHeight();
    chans = bmp.getBitCount() / 8;
    buf = (void*)bmp.getDataRGB();

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // build our texture mipmaps
    switch(chans)
    {
    case 1:
        gluBuild2DMipmaps(GL_TEXTURE_2D, chans, x, y, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf);
        break;
    case 3:
        gluBuild2DMipmaps(GL_TEXTURE_2D, chans, x, y, GL_RGB, GL_UNSIGNED_BYTE, buf);
        break;
    case 4:
        gluBuild2DMipmaps(GL_TEXTURE_2D, chans, x, y, GL_RGBA, GL_UNSIGNED_BYTE, buf);
        break;
    }

    return texture;
}

// rotate the camera
void ModelGL::rotateCamera(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
}

// zoom the camera
void ModelGL::zoomCamera(int delta)
{
    if(mouseRightDown)
    {
        cameraDistance += (delta - mouseY) * 0.05f;
        mouseY = delta;
    }
}

// change drawing mode
void ModelGL::setDrawMode(int mode)
{
    if(drawMode != mode)
    {
        changeDrawMode = true;
        drawMode = mode;
    }
}

