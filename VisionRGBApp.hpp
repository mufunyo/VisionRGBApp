//////////////////////////////////////////////////
// DXApp.h
//  Declaration of the VisionRGBApp class
//  Note: we intentionally don't use the "C" prefix for class names here
//  because we're mostly outside of Win32++ code and we'd potentially
//  be creating name conflicts with existing Win32++ class names.

#ifndef DXAPP_H
#define DXAPP_H

#include "MainFrm.hpp"


class CVRGBApp : public CWinApp
{
public:
    CVRGBApp();
    virtual ~CVRGBApp() {}
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;
};


// returns a pointer to the CDXApp object
inline CVRGBApp* GetDXApp() { return static_cast<CVRGBApp*>(GetApp()); }


#endif
