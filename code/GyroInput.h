#ifndef GYROINPUT_H_
#define GYROINPUT_H_

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <map>

#define GK_POWER    151
#define GK_MUTE     121
#define GK_UP       111
#define GK_DOWN     116
#define GK_LEFT     113
#define GK_RIGHT    114
#define GK_ENTER    36
#define GK_BACK     166
#define GK_HOME     180
#define GK_ZOOMOUT  71
#define GK_ZOOMIN   72
#define GK_VOLDEC   122
#define GK_VOLINC   123
#define GK_ESC      9
#define GK_MENU     135
#define GK_BACKSP   22
#define GK_RWD      173
#define GK_PLAY     172
#define GK_FWD      171

class GyroListener {
public:
	virtual ~GyroListener() {}
	virtual void gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {}
	virtual void gyroKeyPressed(int dev, int keycode) {}
	virtual void gyroKeyReleased(int dev, int keycode) {}
};

class GyroInput {
public:
	GyroInput();
	virtual ~GyroInput();
	int connect(void);
	void capture(void);
	void setEventCallback(GyroListener *gyroListener);
private:
	Display *dis;
	GyroListener *listener;
	Window win;
	int xi_opcode;
	std::map<int, int> devI;
};

#endif /* GYROINPUT_H_ */
