
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"


class CSettingDlg;

class CRecordSettingDlg : public CDialogImpl<CRecordSettingDlg> ,
	public CUpdateUI<CRecordSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{

private:
	CSettingDlg *m_parent;
	CListViewCtrl m_channelList;
	int m_channelIndex[BLM_CHANNEL_MAX];
	BOOL m_localRecodePlan[BLM_CHANNEL_MAX][7][24];
	CButton m_checkButton[7][24];
	int m_selectedChannel;
	CStatic m_hourLabel[24];
	CStatic m_dayLabel[7];
public:
	CRecordSettingDlg(CSettingDlg *parent){m_parent = parent;};
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
		MESSAGE_HANDLER(BM_CONFIRM_SETTING, OnConfirm)
		NOTIFY_HANDLER(IDC_CHANNEL_LIST, LVN_ITEMCHANGED, OnLvnItemchangedChannelList)
	END_MSG_MAP()


	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnConfirm(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLvnItemchangedChannelList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	void initPlanCheck();
	void updatePlanCheckValue();
	void savePlanCheckValue();
	void initList();
};
