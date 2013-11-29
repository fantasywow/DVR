
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"
#include "PreviewDlg.h"

class CSettingDlg;


class CCradleSettingDlg : public CDialogImpl<CCradleSettingDlg> ,
	public CUpdateUI<CCradleSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{

private:
	CSettingDlg *m_parent;
	CListViewCtrl m_channelList;
	int m_selectedChannel;
	CPreiviewDlg m_previewDlg;
public:
	CCradleSettingDlg(CSettingDlg *parent){m_parent = parent;};
	enum { IDD = IDD_CRADLE_SETTING };


	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CCradleSettingDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CCradleSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(BM_CONFIRM_SETTING, OnConfirm)
		NOTIFY_HANDLER(IDC_LIST_CRADLE, LVN_ITEMCHANGED, OnLvnItemchangedListCradle)
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
	void initList();
	LRESULT OnLvnItemchangedListCradle(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
