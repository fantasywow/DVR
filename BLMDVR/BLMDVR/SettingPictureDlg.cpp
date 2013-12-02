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
	GetDlgItem(IDC_CHECK_OSDNAME).EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_OSDTIME).EnableWindow(FALSE);

	initList();
	m_previewDlg.Create(m_hWnd,0);
	m_previewDlg.SetWindowPos(NULL,200,15,264,216,SWP_SHOWWINDOW);


	return true;
}

LRESULT CPictureSettingDlg::OnLvnItemchangedChannellist(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState&LVIS_SELECTED) 
	{
		int index = m_channelList.GetSelectedIndex();
		m_channelNameEdit.EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_OSDNAME).EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_OSDTIME).EnableWindow(TRUE);
		m_osdNameCheck = m_parent->m_osdName[m_channelIndex[index]];
		m_osdTimeCheck = m_parent->m_osdTime[m_channelIndex[index]];
		DoDataExchange(FALSE);
		CString buff;
		m_channelList.GetItemText(index,1,buff);
		m_channelNameEdit.SetWindowText(buff);

		m_previewDlg.changeChannel(m_channelIndex[index]);
		//StartVideoPreview(m_parent->m_channelHandle[i],m_previewDlg,&rc,FALSE,0,25);
	}	
	return 0;

}

LRESULT CPictureSettingDlg::OnEnChangeEditChannelname(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString temp;
	m_channelNameEdit.GetWindowText(temp);
	m_channelList.AddItem(m_channelList.GetSelectedIndex(),1,temp);
	return 0;
}

LRESULT CPictureSettingDlg::OnConfirm( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	DoDataExchange(true);
	int index = m_channelList.GetSelectedIndex();
	if (index != -1)
	{
		m_parent->m_osdName[index]=m_osdNameCheck;
		m_parent->m_osdTime[index]=m_osdTimeCheck;
	}
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

void CPictureSettingDlg::initList()
{
	m_channelList = GetDlgItem(IDC_CHANNEL_LIST);
	m_channelList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_channelList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_channelList.AddColumn("ÐòºÅ", 0);
	m_channelList.AddColumn("Í¨µÀÃû³Æ", 1);
	for (int i=0,j=0;i<BLM_CHANNEL_MAX;i++)
	{
		// 		if (m_parent->m_channelHandle[i]!=INVALID_HANDLE_VALUE)
		// 		{
		CString channelID;
		channelID.Format("%d",i);
		m_channelList.AddItem(j,0,channelID);
		m_channelList.AddItem(j,1,m_parent->m_channelName[i]);
		m_channelIndex[j]=i;
		j++;
		/*		}*/
	}

}

LRESULT CPictureSettingDlg::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	m_previewDlg.DestroyWindow();
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	return 0;
}
