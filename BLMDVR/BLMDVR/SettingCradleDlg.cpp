#include "stdafx.h"
#include "SettingCradleDlg.h"
#include "SettingDlg.h"
#include "PreviewDlg.h"


LRESULT CCradleSettingDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);
	m_selectedChannel = 0;
	initList();
	m_previewDlg.Create(m_hWnd,0);
	m_previewDlg.SetWindowPos(NULL,10,160,264,216,SWP_SHOWWINDOW);

	return true;
}

LRESULT CCradleSettingDlg::OnConfirm( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

void CCradleSettingDlg::initList()
{
	m_channelList = GetDlgItem(IDC_LIST_CRADLE);
	m_channelList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_channelList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_channelList.AddColumn("���", 0);
	m_channelList.AddColumn("ͨ������", 1);
	m_channelList.AddColumn("Э��", 2);
	m_channelList.AddColumn("����", 3);
	m_channelList.AddColumn("������", 4);
	m_channelList.AddColumn("��ַ��", 5);
	m_channelList.AddColumn("��̨�ٶ�", 6);
	m_channelList.AddColumn("��ͷ�ٶ�", 7);
	m_channelList.AddColumn("��̨ת��Ч��", 8);
	for (int i=0,j=0;i<BLM_CHANNEL_MAX;i++)
	{
		//if (m_parent->m_channelHandle[i]!=INVALID_HANDLE_VALUE)
		//{
		CString channelID;
		channelID.Format("%d",i);
		m_channelList.AddItem(i,0,channelID);
		m_channelList.AddItem(i,1,m_parent->m_channelName[i]);
		//m_channelIndex[j]=i;
		//j++;
		//}
	}
}

LRESULT CCradleSettingDlg::OnLvnItemchangedListCradle(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState&LVIS_SELECTED) 
	{
		m_selectedChannel = m_channelList.GetSelectedIndex();   //��õ�����Ǹ���
	}	
	return 0;
}
