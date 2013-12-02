
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
	CSettingDlg* m_parent;
	CListViewCtrl m_channelList;
	CComboBox m_quality,m_quality_sub,m_format,m_format_sub,m_frameRate,m_frameRate_sub,m_maxBit,m_maxBit_sub;
	CButton m_captureAudio,m_captureSub;
	BlmEncodeSetting m_localEncodeSetting[8];
	int m_selectedChannel;
	int m_channelIndex[BLM_CHANNEL_MAX];
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
		COMMAND_HANDLER(IDC_CHECK_SUB, BN_CLICKED, OnBnClickedCheckSub)
	END_MSG_MAP()

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnConfirm(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLvnItemchangedChannelList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckSub(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void updateChooseChannel();
	void initComboBox();
	void checkSub();
	void saveValue();
};
