#include "stdafx.h"
#include "SettingEncodeDlg.h"




LRESULT CEncodeSettingDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);
	
	m_channelList.Attach(GetDlgItem(IDC_CHANNEL_LIST));
	m_channelList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_channelList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_channelList.AddColumn(L"序号", 0);
	m_channelList.AddColumn(L"通道名称", 1);

	m_quality.Attach(GetDlgItem(IDC_COMBO_QUALITY));
	m_quality.AddString(L"最好");
	m_quality.AddString(L"较好");
	m_quality.AddString(L"一般");
	m_quality.AddString(L"较差");
	m_quality.AddString(L"最差");
	m_quality_sub.Attach(GetDlgItem(IDC_COMBO_QUALITY_SUB));
	m_quality_sub.AddString(L"最好");
	m_quality_sub.AddString(L"较好");
	m_quality_sub.AddString(L"一般");
	m_quality_sub.AddString(L"较差");
	m_quality_sub.AddString(L"最差");
	m_format.Attach(GetDlgItem(IDC_COMBO_FORMAT));
	m_format.AddString(L"CIF");
	m_format.AddString(L"QCIF");
	m_format.AddString(L"2CIF");
	m_format.AddString(L"D1");
	m_format_sub.Attach(GetDlgItem(IDC_COMBO_FORMAT_SUB));
	m_format_sub.AddString(L"CIF");
	m_format_sub.AddString(L"QCIF");
	m_format_sub.AddString(L"2CIF");
	m_format_sub.AddString(L"D1");
	m_frameRate.Attach(GetDlgItem(IDC_COMBO_FRAMERATE));
	m_frameRate.AddString(L"25");
	m_frameRate_sub.Attach(GetDlgItem(IDC_COMBO_FRAMERATE_SUB));
	m_frameRate_sub.AddString(L"25");
	m_maxBit.Attach(GetDlgItem(IDC_COMBO_MAXBIT));
	m_maxBit.AddString(L"300");
	m_maxBit_sub.Attach(GetDlgItem(IDC_COMBO_MAXBIT_SUB));
	m_maxBit_sub.AddString(L"300");

	m_selectedChannel = 0;
	//m_encodeSetting[m_selectedChannel]={0,0,0,0,0,0,0,0,0,0};

	return true;
}

