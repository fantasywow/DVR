
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"
#include "BlmTypes.h"

class CSettingDlg;


class CEncodeSettingDlg : public CDialogImpl<CEncodeSettingDlg> ,
	public CUpdateUI<CEncodeSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{

private:
	CListViewCtrl m_channelList;
	CComboBox m_quality,m_quality_sub,m_format,m_format_sub,m_frameRate,m_frameRate_sub,m_maxBit,m_maxBit_sub;
	BlmEncodeSetting m_encodeSetting[8];
	int m_selectedChannel;
	CSettingDlg* m_parent;
public:
	CEncodeSettingDlg(CSettingDlg *parent){m_parent = parent;};
	enum { IDD = IDD_ENCODE_SETTING };

	
	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CEncodeSettingDlg)
	END_UPDATE_UI_MAP()

	

	BEGIN_MSG_MAP(CEncodeSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(BM_CONFIRM_SETTING, OnConfirm)
		NOTIFY_HANDLER(IDC_CHANNEL_LIST, LVN_ITEMCHANGED, OnLvnItemchangedChannelList)
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
	LRESULT OnLvnItemchangedChannelList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
