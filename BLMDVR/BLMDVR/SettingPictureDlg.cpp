#include "stdafx.h"
#include "SettingPictureDlg.h"
#include "SettingDlg.h"



LRESULT CPictureSettingDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);

	m_channelNameEdit = GetDlgItem(IDC_EDIT_CHANNELNAME);
	m_channelNameEdit.EnableWindow(FALSE);
	m_channelList = GetDlgItem(IDC_CHANNEL_LIST);
	m_channelList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_channelList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_channelList.AddColumn(L"ÐòºÅ", 0);
	m_channelList.AddColumn(L"Í¨µÀÃû³Æ", 1);
	for (int i=0,j=0;i<BLM_CHANNEL_MAX;i++)
	{
// 		if (m_parent->m_channelHandle[i]!=INVALID_HANDLE_VALUE)
// 		{
			CString channelID;
			channelID.Format(L"%d",i);
			m_channelList.AddItem(j,0,channelID);
			m_channelList.AddItem(j,1,m_parent->m_channelName[i]);
			j++;
/*		}*/
	}
	return true;
}

LRESULT CPictureSettingDlg::OnLvnItemchangedChannellist(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState&LVIS_SELECTED) 
	{
		m_channelNameEdit.EnableWindow(TRUE);
		CString buff;
		m_channelList.GetItemText(m_channelList.GetSelectedIndex(),1,buff);
		m_channelNameEdit.SetWindowText(buff);
	}	
	return 0;

}

LRESULT CPictureSettingDlg::OnEnChangeEditChannelname(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ATLTRACE(L"test");
	CString temp;
	m_channelNameEdit.GetWindowText(temp);
	m_channelList.AddItem(m_channelList.GetSelectedIndex(),1,temp);
	return 0;
}

LRESULT CPictureSettingDlg::OnConfirm( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	for (int i=0,j=0;i<BLM_CHANNEL_MAX;i++)
	{
// 		if (m_parent->m_channelHandle[i]!=INVALID_HANDLE_VALUE)
// 		{
			m_channelList.GetItemText(j,1,m_parent->m_channelName[i]);
			j++;
/*		}*/
	}
	return 0;
}
