#include "stdafx.h"
#include "SettingRecordDlg.h"
#include "SettingDlg.h"



LRESULT CRecordSettingDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);
	m_selectedChannel = -1;
	memcpy(m_localRecodePlan,m_parent->m_recodePlan,sizeof(BOOL)*BLM_CHANNEL_MAX*7*24);
	m_channelList = GetDlgItem(IDC_CHANNEL_LIST);
	m_channelList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_channelList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_channelList.AddColumn(L"序号", 0);
	m_channelList.AddColumn(L"通道名称", 1);
	for (int i=0,j=0;i<BLM_CHANNEL_MAX;i++)
	{
		//if (m_parent->m_channelHandle[i]!=INVALID_HANDLE_VALUE)
		//{
		CString channelID;
		channelID.Format(L"%d",i);
		m_channelList.AddItem(j,0,channelID);
		m_channelList.AddItem(j,1,m_parent->m_channelName[i]);
		m_channelIndex[j]=i;
		j++;
		//}
	}

	initPlanCheck();
	updatePlanCheckValue();
	return true;
}

LRESULT CRecordSettingDlg::OnConfirm( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	savePlanCheckValue();
	memcpy(m_parent->m_recodePlan,m_localRecodePlan,sizeof(BOOL)*BLM_CHANNEL_MAX*7*24);
	return 0;
}

LRESULT CRecordSettingDlg::OnLvnItemchangedChannelList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState&LVIS_SELECTED) 
	{
		if (m_selectedChannel != -1)
		{
			savePlanCheckValue();
		}
		m_selectedChannel = m_channelList.GetSelectedIndex();   //获得点击的那个行
		updatePlanCheckValue();
	}	

	return 0;

}

void CRecordSettingDlg::initPlanCheck()
{
	RECT rc ={200,30,200+20,30+20};
	for (int i= 0;i<7;i++)
	{
		for(int j=0;j<24;j++)
		{
			m_checkButton[i][j].Create(m_hWnd, rc, _T("Checkbox"),WS_CHILD | WS_VISIBLE|BS_AUTOCHECKBOX);
			m_checkButton[i][j].SetWindowText(L"");
			rc.left+=20;
			rc.right+=20;
		}
		rc.left = 200;
		rc.right= 200+20;
		rc.bottom+=20;
		rc.top+=20;
	}


	updatePlanCheckValue();
}

void CRecordSettingDlg::updatePlanCheckValue()
{
	for (int i= 0;i<7;i++)
	{
		for(int j=0;j<24;j++)
		{
			if (m_selectedChannel == -1)
			{
				m_checkButton[i][j].EnableWindow(FALSE);
			}else{
				m_checkButton[i][j].EnableWindow(TRUE);
				m_checkButton[i][j].SetCheck(m_localRecodePlan[m_channelIndex[m_selectedChannel]][i][j]);
			}		
		}
	}
}

void CRecordSettingDlg::savePlanCheckValue()
{
	for (int i= 0;i<7;i++)
	{
		for(int j=0;j<24;j++)
		{
			if (m_selectedChannel!=-1)
			{
				m_localRecodePlan[m_channelIndex[m_selectedChannel]][i][j] = m_checkButton[i][j].GetCheck();	
			}
		}
	}
}

void CRecordSettingDlg::clearPlanCheck()
{
	for (int i= 0;i<7;i++)
	{
		for(int j=0;j<24;j++)
		{
			m_checkButton[i][j].DestroyWindow();	
		}
	}
}

