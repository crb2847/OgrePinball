#ifndef GYROINPUT_H_
#define GYROINPUT_H_

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <map>

class GyroListener {
public:
	virtual ~GyroListener() {}
	virtual void gyroMoved(int dev, double x, double y, double raw_x, double raw_y) = 0;
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
