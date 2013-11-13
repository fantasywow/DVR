
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"


class CSettingDlg;


class CPictureSettingDlg : public CDialogImpl<CPictureSettingDlg> ,
	public CUpdateUI<CPictureSettingDlg>,
	public CWinDataExchange<CPictureSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{

private:
	CEdit m_channelNameEdit;
	CListViewCtrl m_channelList;
	CSettingDlg *m_parent;
	int m_osdNameCheck,m_osdTimeCheck;
	int m_channelIndex[BLM_CHANNEL_MAX];
public:
	CPictureSettingDlg(CSettingDlg *parent){m_parent = parent;};
	enum { IDD = IDD_PICTURE_SETTING };


	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CPictureSettingDlg)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CPictureSettingDlg)
		DDX_CHECK(IDC_CHECK_OSDNAME,m_osdNameCheck)
		DDX_CHECK(IDC_CHECK_OSDTIME,m_osdTimeCheck)
	END_DDX_MAP()


	BEGIN_MSG_MAP(CPictureSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(BM_CONFIRM_SETTING, OnConfirm)
		NOTIFY_HANDLER(IDC_CHANNEL_LIST, LVN_ITEMCHANGED, OnLvnItemchangedChannellist)
		COMMAND_HANDLER(IDC_EDIT_CHANNELNAME, EN_CHANGE, OnEnChangeEditChannelname)
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
	LRESULT OnLvnItemchangedChannellist(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditChannelname(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
