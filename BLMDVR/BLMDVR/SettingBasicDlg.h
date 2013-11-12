
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"


class CSettingDlg;


class CBasicSettingDlg : public CDialogImpl<CBasicSettingDlg> ,
	public CUpdateUI<CBasicSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{

private:
	CListViewCtrl m_diskList;
	CSettingDlg *m_parent;
public:
	CBasicSettingDlg(CSettingDlg *parent){m_parent = parent;};

	enum { IDD = IDD_BASIC_SETTING };

	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CBasicSettingDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CBasicSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(BM_CONFIRM_SETTING, OnConfirm);
		NOTIFY_HANDLER(IDC_LIST_DISK, LVN_ITEMCHANGED, OnLvnItemchangedListDisk)
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
	LRESULT OnConfirm(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLvnItemchangedListDisk(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
