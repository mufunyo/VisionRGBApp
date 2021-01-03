//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "View.hpp"
#include "resource.h"

CView::CView()
{
}

int CView::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    return 0;
}

void CView::OnInitialUpdate()
// OnInitialUpdate is called immediately after the window is created
{
    TRACE("View window created\n");
}

inline LRESULT CView::OnNotify(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(wparam);

    LPNMHDR pNMHDR = (LPNMHDR)lparam;
    switch (pNMHDR->code)
    {
        // Pass the ToolBar's ToolTip info up to the frame
    case TTN_GETDISPINFO: return GetParent().SendMessage(WM_NOTIFY, wparam, lparam);
    }

    return 0;
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Here we set the defaults used by the create function for the view window
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the window we create.
}

void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Here we set the Window class parameters.
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the type of window we create.

    // Set the Window Class name
    wc.lpszClassName = _T("Win32++ View");

    // Set a background brush to white
    wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

    // Set the default cursor
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

    // Set the class style (not to be confused with the window styles set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// All window messages for this window pass through WndProc
{
    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}



