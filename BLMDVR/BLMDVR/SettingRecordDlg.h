
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"

class CRecordSettingDlg : public CDialogImpl<CRecordSettingDlg> ,
	public CUpdateUI<CRecordSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{

private:

public:
	enum { IDD = IDD_RECORD_SETTING };


	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CRecordSettingDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CRecordSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()


	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

};
