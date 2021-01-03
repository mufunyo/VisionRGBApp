//////////////////////////////////////////////////
// DXApp.h
//  Declaration of the VisionRGBApp class
//  Note: we intentionally don't use the "C" prefix for class names here
//  because we're mostly outside of Win32++ code and we'd potentially
//  be creating name conflicts with existing Win32++ class names.

#ifndef DXAPP_H
#define DXAPP_H

#include "MainFrm.hpp"
#include "Wrappers.hpp"


class VisionRGBApp : public CWinApp
{
public:
    VisionRGBApp();
    virtual ~VisionRGBApp() {}
    virtual BOOL InitInstance();
    virtual int MessageLoop();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
    D3D9Context m_d3D9;
};

#endif
