#include "GyroInput.h"
#include <cstring>
#include <cstdio>
#include <stdint.h>

GyroInput::GyroInput() : listener(nullptr){
	dis = XOpenDisplay(0);
	win = DefaultRootWindow(dis);
	int event, error;
	XQueryExtension(dis, "XInputExtension", &xi_opcode, &event, &error);
}

GyroInput::~GyroInput() {
	XCloseDisplay(dis);
}

int GyroInput::connect(void) {
	int devCnt[2] = {0, 0};
	XIDeviceInfo *info; int n, i;
	info = XIQueryDevice(dis, XIAllDevices, &n);
	for (i = 0; i < n; i++) {
		XIDeviceInfo *dev = &info[i];
		if (strcmp(dev->name, "RCMCU iPazzPort") != 0) continue;
		if (dev->use != XIFloatingSlave) {
			XIDetachSlaveInfo dsi = {XIDetachSlave, dev->deviceid};
			XIChangeHierarchy(dis, (XIAnyHierarchyChangeInfo *) &dsi, 1);
		}
		int type = 0; //0=keyboard, 1=mouse
		for (int j = 0; j < dev->num_classes; j++)
			if (dev->classes[j]->type == XIValuatorClass) { type = 1; break; }
		devI[dev->deviceid] = ++devCnt[type];
		uint32_t mask = XI_RawMotionMask  | XI_RawKeyPressMask | XI_RawKeyReleaseMask;
		XIEventMask evtMask {dev->deviceid, 4, (unsigned char *) &mask};
		XISelectEvents(dis, win, &evtMask, 1);
	}
	XIFreeDeviceInfo(info);
	return devCnt[1];
}

void GyroInput::capture(void) {
	XEvent ev;
	while(XPending(dis) > 0) {
		XNextEvent(dis, &ev);
		if (ev.xcookie.type == GenericEvent && ev.xcookie.extension == xi_opcode &&
				XGetEventData(dis, &ev.xcookie) && listener != nullptr) {
			XIRawEvent *re =  (XIRawEvent *) ev.xcookie.data;
			if (ev.xcookie.evtype == XI_RawMotion) {
				XIRawEvent *re =  (XIRawEvent *) ev.xcookie.data;
				double dx = 0.0, dy = 0.0, dxr = 0.0, dyr = 0.0;
				if (re->valuators.mask[0] & 1) { dx = re->valuators.values[0]; dxr = re->raw_values[0]; }
				if (re->valuators.mask[0] & 2) { dy = re->valuators.values[1]; dyr = re->raw_values[1]; }
				listener->gyroMoved(devI[re->deviceid], dx, dy, dxr, dyr);
			} else if (ev.xcookie.evtype == XI_RawKeyPress) {
				listener->gyroKeyPressed(devI[re->deviceid], re->detail);
			} else if (ev.xcookie.evtype == XI_RawKeyRelease) {
				listener->gyroKeyReleased(devI[re->deviceid], re->detail);
			}
		}
		XFreeEventData(dis, &ev.xcookie);
	}
}

void GyroInput::setEventCallback(GyroListener *gyroListener) {
	listener = gyroListener;
}
