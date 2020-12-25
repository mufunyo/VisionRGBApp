//////////////////////////////////////////////////
// DXApp.h
//  Declaration of the CDXApp class

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
