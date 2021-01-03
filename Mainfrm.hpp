/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.hpp"
#include "SettingsDialog.hpp"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    BOOL    OnFileExit();
    BOOL    OnFileOpen();
    BOOL    OnFilePrint();
    BOOL    OnFileSave();
    BOOL    OnSettings();
    BOOL    OnTBBigIcons();
    BOOL    OnTBCustomize();
    BOOL    OnTBDefault();
    void    SaveTBDefault();
    void    AddInputList();
    void    SetupStatusBar();
    HWND    GetView();

protected:
    virtual LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual LRESULT OnDrawItem(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnCustHelp(LPNMHDR pNMHDR);
    virtual LRESULT OnEndAdjust(LPNMHDR pNMHDR);
    virtual LRESULT OnGetButtonInfo(LPNMTOOLBAR pNMTB);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnQueryDelete(LPNMTOOLBAR pNMTB);
    virtual LRESULT OnQueryInsert(LPNMTOOLBAR pNMTB);
    virtual LRESULT OnReset(LPNMTOOLBAR pNMTB);
    virtual LRESULT OnToolBarChange(LPNMTOOLBAR pNMTB);
    virtual void SetupToolBar();
    CSettingsDialog& GetDialog() { return m_settingsDialog; }
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    std::vector<TBBUTTON>   m_defaultButtons;
    std::vector<TBBUTTON>   m_resetButtons;
    BOOL    m_useBigIcons;
    CToolBar m_inputBar;
    CComboBox m_inputList;
    CSettingsDialog m_settingsDialog;
};

HFONT GetWindowsDefaultFont();

#endif //MAINFRM_H
