//////////////////////////////////////////////////
// VisionRGBApp.h
//  Declaration of the VisionRGBApp class
//  Note: we intentionally don't use the "C" prefix for class names here
//  because we're mostly outside of Win32++ code and we'd potentially
//  be creating name conflicts with existing Win32++ class names.

#ifndef DXAPP_H
#define DXAPP_H

#include "MainFrm.hpp"
#include "Wrappers.hpp"

#include <queue>

// RGBEasy SDK
#include <rgb.h>
#include <rgbapi.h>
#include <rgberror.h>

#define NUM_BUFFERS 2

struct CapturedFrame
{
	D3D9Surface* pSurface;
	unsigned long newWidth;
	unsigned long newHeight;
};

enum SignalState
{
	active,
	inactive,
	invalid
};

class VisionRGBApp : public CWinApp
{
public:
    VisionRGBApp();
    virtual ~VisionRGBApp() {}
    virtual BOOL InitInstance();
    virtual int MessageLoop();
    CMainFrame& GetMainFrame() { return m_frame; }

	void recvSample(PRGBFRAMEDATA pFrameData);
	void drawSample();
	bool startCapture(int width, int height, PixelFmt pixFormat);
	void stopCapture();

	bool bCapturing;
	D3D9Surface* pSurfaces[NUM_BUFFERS];
	unsigned long buffers;
	std::queue<CapturedFrame> qFrames;
	SignalState signal;
	Mutex mutex;
	PixelFmt format;
	unsigned long width;
	unsigned long height;

private:
    CMainFrame m_frame;
    D3D9Context m_d3D9;
};

#endif
