//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "resource.h"

CView::CView()
{
}

BOOL CView::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Process the messages from the child ToolBar
    UNREFERENCED_PARAMETER(lparam);

    switch(LOWORD(wparam))
    {
    case IDM_TOP:       OnTop();        return TRUE;
    case IDM_LEFT:      OnLeft();       return TRUE;
    case IDM_RIGHT:     OnRight();      return TRUE;
    case IDM_BOTTOM:    OnBottom();     return TRUE;
    }

    return FALSE;
}

void CView::OnBottom()
{
    DWORD style = m_toolBar.GetStyle();

    style &= ~(CCS_VERT);
    style |= CCS_BOTTOM;
    m_toolBar.SetStyle(style);
    RecalcLayout();
}

void CView::OnLeft()
{
    DWORD style = m_toolBar.GetStyle();

    style &= ~(CCS_BOTTOM);
    style |= CCS_LEFT;
    m_toolBar.SetStyle(style);
    RecalcLayout();
}

void CView::OnRight()
{
    DWORD style = m_toolBar.GetStyle();

    style |= CCS_RIGHT;
    m_toolBar.SetStyle(style);
    RecalcLayout();
}

void CView::OnTop()
{
    DWORD style = m_toolBar.GetStyle();

    style &= ~(CCS_VERT | CCS_BOTTOM);
    m_toolBar.SetStyle(style);
    RecalcLayout();
}

int CView::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    // Create the ToolBar's image list from 4 icons
    m_toolBarImages.Create(48, 48, ILC_COLOR32 | ILC_MASK, 0, 0);
    m_toolBarImages.Add(GetApp()->LoadIcon(IDI_TOP));
    m_toolBarImages.Add(GetApp()->LoadIcon(IDI_LEFT));
    m_toolBarImages.Add(GetApp()->LoadIcon(IDI_RIGHT));
    m_toolBarImages.Add(GetApp()->LoadIcon(IDI_BOTTOM));

    // Create the ToolBar
    m_toolBar.Create(*this);
    m_toolBar.SetImageList(m_toolBarImages);

    DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS
                    | TBSTYLE_FLAT | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER ;

    m_toolBar.SetStyle(style);

    // Add the ToolBar buttons
    TBBUTTON buttonInfo[] =
    {
        { 0, IDM_TOP,       TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
        { 1, IDM_LEFT,      TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
        { 2, IDM_RIGHT,     TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
        { 3, IDM_BOTTOM,    TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 }
    };
    m_toolBar.AddButtons(4, buttonInfo);

    return 0;
}

void CView::OnDraw(CDC& dc)
// OnDraw is called when part or all of the window needs to be redrawn
{
    CRect clientRect = GetClientRect();
    CRect tbRect = m_toolBar.GetWindowRect();

    // Calculate the view rect excluding the ToolBar rect
    ClientToScreen(clientRect);
    if (clientRect.Width() == tbRect.Width())
    {
        if (clientRect.top == tbRect.top)
            clientRect.top += tbRect.Height();
        else
            clientRect.bottom -= tbRect.Height();
    }
    else
    {
        if (clientRect.left == tbRect.left)
            clientRect.left += tbRect.Width();
        else
            clientRect.right -= tbRect.Width();
    }

    ScreenToClient(clientRect);

    // Display some text in our view window
    CString text = "\nPress the arrows to change the inner toolbar's orientation,";
    text += "\n or choose customize toolbar from the ToolBar menu item to modify the toolbar in the frame.";
    dc.DrawText(text, -1, clientRect, DT_CENTER | DT_WORDBREAK);
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

    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
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

void CView::RecalcLayout()
{
    // Position the toolbar at the top, left, right or bottom of the view.
    int cxTB = m_toolBar.GetMaxSize().cx;
    int cyTB = m_toolBar.GetMaxSize().cy;
    int cxClient = GetClientRect().Width();
    int cyClient = GetClientRect().Height();

    DWORD style = m_toolBar.GetStyle();
    style &= CCS_VERT | CCS_BOTTOM; // Filter unwanted styles

    switch(style)
    {
    case CCS_LEFT:
        m_toolBar.SetWindowPos(NULL, 0, 0, cxTB, cyClient, SWP_SHOWWINDOW);
        SetWrapState(TRUE);
        m_toolBar.PressButton(IDM_LEFT, TRUE);
        break;
    case CCS_RIGHT:
        m_toolBar.SetWindowPos(NULL, cxClient - cxTB, 0, cxTB, cyClient, SWP_SHOWWINDOW);
        SetWrapState(TRUE);
        m_toolBar.PressButton(IDM_RIGHT, TRUE);
        break;
    case CCS_BOTTOM:
        m_toolBar.SetWindowPos(NULL, 0, cyClient - cyTB, cxClient, cyTB, SWP_SHOWWINDOW);
        SetWrapState(FALSE);
        m_toolBar.PressButton(IDM_BOTTOM, TRUE);
        break;
    default:
        m_toolBar.SetWindowPos(NULL, 0, 0, cxClient, cyTB, SWP_SHOWWINDOW);
        SetWrapState(FALSE);
        m_toolBar.PressButton(IDM_TOP, TRUE);
        break;
    }

    Invalidate();
}

// Note: Vertical toolbars require each button to have TBSTATE_WRAP, set for group buttons to work.
//       Horizontal toolbars require the TBSTATE_WRAP removed.
void CView::SetWrapState(BOOL isWrapped)
{
    for (int i = 0; i < m_toolBar.GetButtonCount(); ++i)
    {
        int id = m_toolBar.GetCommandID(i);
        m_toolBar.SetButtonState(id, TBSTATE_ENABLED | (isWrapped ? TBSTATE_WRAP : 0));
    }
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// All window messages for this window pass through WndProc
{
    switch (msg)
    {
    case WM_SIZE:
        RecalcLayout();
        Invalidate();
        break;
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}



