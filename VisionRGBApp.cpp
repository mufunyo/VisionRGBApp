//////////////////////////////////////////////
// DXApp.cpp

#include "stdafx.h"
#include "VisionRGBApp.hpp"
#include "Error.hpp"


VisionRGBApp::VisionRGBApp()
{
    //Create the Window
    m_frame.Create();
    m_d3D9 = D3D9Context(m_frame.GetView());
}

BOOL VisionRGBApp::InitInstance()
{
    return TRUE;
}

int VisionRGBApp::MessageLoop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    D3D9Surface* testImage = m_d3D9.createSurfaceFromFile(TEXT("..\\..\\VisionRGBApp.png"));
    m_d3D9.blit(testImage, &m_d3D9.backBuf);

    while (1) {
        // for some reason, present() is slow (even when not presenting anything at all),
        // so for now let's only present if there are no messages.
        if (!::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            m_d3D9.present();
            Sleep(0);
            continue;
        }
        if (msg.message == WM_QUIT)
            break;

        if (!PreTranslateMessage(msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
    return LOWORD(msg.wParam);
}
