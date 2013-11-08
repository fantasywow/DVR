
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"
#include "BlmTypes.h"



class CEncodeSettingDlg : public CDialogImpl<CEncodeSettingDlg> ,
	public CUpdateUI<CEncodeSettingDlg>,
	public CWinDataExchange<CEncodeSettingDlg>,
	//public CPropertyPageImpl<CEncodeSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{

private:
	CListViewCtrl m_channelList;
	CComboBox m_quality,m_quality_sub,m_format,m_format_sub,m_frameRate,m_frameRate_sub,m_maxBit,m_maxBit_sub;
	BlmEncodeSetting m_encodeSetting[8];
	int m_selectedChannel;
public:
	enum { IDD = IDD_ENCODE_SETTING };


	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CEncodeSettingDlg)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CEncodeSettingDlg)		
		/*
		//DDX_CONTROL_HANDLE(IDC_COMBO_QUALITY,m_quality)
		//DDX_CHECK(IDC_CHECK_AUDIO, m_encodeSetting[m_selectedChannel].audio)
		DDX_COMBO_INDEX(IDC_COMBO_QUALITY, m_encodeSetting[m_selectedChannel].quality)
		DDX_COMBO_INDEX(IDC_COMBO_FORMAT, m_encodeSetting[m_selectedChannel].format)
		DDX_COMBO_INDEX(IDC_COMBO_FRAMERATE, m_encodeSetting[m_selectedChannel].frameRate)
		DDX_COMBO_INDEX(IDC_COMBO_MAXBIT, m_encodeSetting[m_selectedChannel].maxBit)
		//DDX_CHECK(IDC_CHECK_SUB, m_encodeSetting[m_selectedChannel].isSub)
		DDX_COMBO_INDEX(IDC_COMBO_QUALITY_SUB, m_encodeSetting[m_selectedChannel].quality_sub)
		DDX_COMBO_INDEX(IDC_COMBO_FORMAT_SUB, m_encodeSetting[m_selectedChannel].format_sub)
		DDX_COMBO_INDEX(IDC_COMBO_FRAMERATE_SUB, m_encodeSetting[m_selectedChannel].frameRate_sub)
		DDX_COMBO_INDEX(IDC_COMBO_MAXBIT_SUB, m_encodeSetting[m_selectedChannel].maxBit_sub)
		*/
	END_DDX_MAP()

	BEGIN_MSG_MAP(CEncodeSettingDlg)
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
