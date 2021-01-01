///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "VisionRGBApp.hpp"


// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CVRGBApp VisionRGBApp;

        // Run the application
        return VisionRGBApp.Run();
    }
    
    // catch all unhandled CException types
    catch (const CException &e)
    {
        // Display the exception and quit
        MessageBox(nullptr, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}
