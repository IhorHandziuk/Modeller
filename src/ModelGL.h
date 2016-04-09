#pragma once
#include <vector>
#include "Shape.h"
using std::vector;

class ModelGL
{
public:
    ModelGL();
    ~ModelGL();

	void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
	void setCameraAngle(float x, float y) { cameraAngleX = x; cameraAngleY = y; };
    void setViewport(int width, int height);

    void setMouseLeft(bool flag) { mouseLeftDown = flag; };
	void setMouseRight(bool flag) { mouseRightDown = flag; };
	void setPicking(bool flag) { pick = flag; };
	void setControl(bool flag) { ctrl = flag; };
	bool isControl() { return ctrl; };
	void hide_grid() { hideGrid = !hideGrid; };
    void setMousePosition(int x, int y) { mouseX = x; mouseY = y; };
    void setDrawMode(int mode);

    void rotateCamera(int x, int y);
    void zoomCamera(int dist);

	void init();
	void draw();
	void  resizeWindow(int w, int h);


	vector<Shape*> shapes;
	float cameraAngleX;
	float cameraAngleY;
protected:

private:
    // member functions
    unsigned int loadTextureBmp(const char *filename);

    // members
	int windowHeight;
	int windowWidth;
	bool windowResized;
	bool changeDrawMode;
	bool pick;
	bool ctrl;
	bool hideGrid;
    int drawMode;
    unsigned int listId;                            // display list ID
    bool mouseLeftDown;
    bool mouseRightDown;
    int mouseX;
    int mouseY;
   
    float cameraDistance;
    unsigned char* frameBuffer;                     // framebuffer to store RGBA color
    int bufferSize;                                 // framebuffer size in bytes

	Grid axes;
	bool isAxes;
};