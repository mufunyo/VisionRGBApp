//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


// Declaration of the CView class
class CView : public CWnd
{
public:
    CView();
    virtual ~CView() {}

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
};


#endif // VIEW_H
