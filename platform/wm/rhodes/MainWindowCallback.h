#ifndef MAINWINDOWCALLBACK_H
#define MAINWINDOWCALLBACK_H

class IMainWindowCallback
{
public:
	virtual void updateSizeProperties(int width, int height) = 0;
	virtual void onActivate(int active) = 0;
protected:
	virtual ~IMainWindowCallback() {};
};

#endif // MAINWINDOWCALLBACK_H
