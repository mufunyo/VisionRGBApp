/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.hpp"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    CDXView& GetDXView() const { return const_cast<CDXView&>(m_dxView); }

    BOOL    OnFileExit();
    BOOL    OnFileOpen();
    BOOL    OnFilePrint();
    BOOL    OnFileSave();
    BOOL    OnTBBigIcons();
    BOOL    OnTBCustomize();
    BOOL    OnTBDefault();
    void    SaveTBDefault();
    void    AddInputList();

protected:
    virtual LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
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
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDXView m_dxView;
    std::vector<TBBUTTON>   m_defaultButtons;
    std::vector<TBBUTTON>   m_resetButtons;
    BOOL    m_useBigIcons;
    CComboBox m_inputList;
};

#endif //MAINFRM_H

