#ifndef MAINWINDOWCALLBACK_H
#define MAINWINDOWCALLBACK_H

#include <string>

class IMainWindowCallback
{
public:
	virtual void updateSizeProperties(int width, int height) = 0;
	virtual void onActivate(int active) = 0;
	virtual void logEvent(const ::std::string& message) = 0;
protected:
	virtual ~IMainWindowCallback() {};
};

#endif // MAINWINDOWCALLBACK_H
