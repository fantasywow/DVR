#include "stdafx.h"
#include "SettingEncodeDlg.h"
#include "SettingDlg.h"



LRESULT CEncodeSettingDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);
	
	m_selectedChannel = -1;
	m_channelList = GetDlgItem(IDC_CHANNEL_LIST);
	m_channelList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_channelList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_channelList.AddColumn(L"���", 0);
	m_channelList.AddColumn(L"ͨ������", 1);
	for (int i=0,j=0;i<BLM_CHANNEL_MAX;i++)
	{
		if (m_parent->m_channelHandle[i]!=INVALID_HANDLE_VALUE)
		{
			CString channelID;
			channelID.Format(L"%d",i);
			m_channelList.AddItem(j,0,channelID);
			m_channelList.AddItem(j,1,m_parent->m_channelName[i]);
		}
	}

	m_quality=GetDlgItem(IDC_COMBO_QUALITY);
	m_quality.AddString(L"���");
	m_quality.AddString(L"�Ϻ�");
	m_quality.AddString(L"һ��");
	m_quality.AddString(L"�ϲ�");
	m_quality.AddString(L"���");
	m_quality_sub=GetDlgItem(IDC_COMBO_QUALITY_SUB);
	m_quality_sub.AddString(L"���");
	m_quality_sub.AddString(L"�Ϻ�");
	m_quality_sub.AddString(L"һ��");
	m_quality_sub.AddString(L"�ϲ�");
	m_quality_sub.AddString(L"���");
	m_format=GetDlgItem(IDC_COMBO_FORMAT);
	m_format.AddString(L"CIF");
	m_format.AddString(L"QCIF");
	m_format.AddString(L"2CIF");
	m_format.AddString(L"D1");
	m_format_sub=GetDlgItem(IDC_COMBO_FORMAT_SUB);
	m_format_sub.AddString(L"CIF");
	m_format_sub.AddString(L"QCIF");
	m_format_sub.AddString(L"2CIF");
	m_format_sub.AddString(L"D1");
	m_frameRate=GetDlgItem(IDC_COMBO_FRAMERATE);
	m_frameRate.AddString(L"25");
	m_frameRate_sub=GetDlgItem(IDC_COMBO_FRAMERATE_SUB);
	m_frameRate_sub.AddString(L"25");
	m_maxBit=GetDlgItem(IDC_COMBO_MAXBIT);
	m_maxBit.AddString(L"300");
	m_maxBit_sub=GetDlgItem(IDC_COMBO_MAXBIT_SUB);
	m_maxBit_sub.AddString(L"300");


	return true;
}


LRESULT CEncodeSettingDlg::OnLvnItemchangedChannelList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMLV->uNewState&LVIS_SELECTED) 
	{
		m_selectedChannel = m_channelList.GetSelectedIndex();   //��õ�����Ǹ���
		
	}	
	return 0;
}

LRESULT CEncodeSettingDlg::OnConfirm( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}
