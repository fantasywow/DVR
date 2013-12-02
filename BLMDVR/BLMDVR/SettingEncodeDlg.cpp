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
	memcpy(m_localEncodeSetting,m_parent->m_encodeSetting,sizeof(BlmEncodeSetting)*BLM_CHANNEL_MAX);
	m_selectedChannel = -1;
	m_channelList = GetDlgItem(IDC_CHANNEL_LIST);
	m_channelList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_channelList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_channelList.AddColumn("序号", 0);
	m_channelList.AddColumn("通道名称", 1);
	for (int i=0,j=0;i<BLM_CHANNEL_MAX;i++)
	{
		//if (m_parent->m_channelHandle[i]!=INVALID_HANDLE_VALUE)
		//{
			CString channelID;
			channelID.Format("%d",i);
			m_channelList.AddItem(j,0,channelID);
			m_channelList.AddItem(j,1,m_parent->m_channelName[i]);
			m_channelIndex[j]=i;
			j++;
		//}
	}
	initComboBox();
	return true;
}


LRESULT CEncodeSettingDlg::OnLvnItemchangedChannelList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState&LVIS_SELECTED) 
	{
		saveValue();
		m_selectedChannel = m_channelList.GetSelectedIndex();   //获得点击的那个行
		updateChooseChannel();
	}	
	return 0;
}

LRESULT CEncodeSettingDlg::OnConfirm( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	saveValue();
	memcpy(m_parent->m_encodeSetting,m_localEncodeSetting,sizeof(BlmEncodeSetting)*BLM_CHANNEL_MAX);
	return 0;
}

void CEncodeSettingDlg::updateChooseChannel()
{
	m_captureAudio.EnableWindow(TRUE);
	m_captureSub.EnableWindow(TRUE);
	m_quality.EnableWindow(TRUE);
	m_format.EnableWindow(TRUE);
	m_frameRate.EnableWindow(TRUE);
	m_maxBit.EnableWindow(TRUE);
	m_captureAudio.SetCheck(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].audio);
	m_captureSub.SetCheck(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].sub);
	m_quality.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].quality);
	m_quality_sub.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].quality_sub);
	m_format.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].format);
	m_format_sub.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].format_sub);
	m_frameRate.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].frameRate);
	m_frameRate_sub.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].frameRate);
	m_maxBit.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].maxBit);
	m_maxBit_sub.SetCurSel(m_localEncodeSetting[m_channelIndex[m_selectedChannel]].maxBit_sub);
	checkSub();
}

void CEncodeSettingDlg::initComboBox()
{
	m_captureAudio = GetDlgItem(IDC_CHECK_AUDIO);
	m_captureAudio.EnableWindow(FALSE);
	m_captureSub = GetDlgItem(IDC_CHECK_SUB);
	m_captureSub.EnableWindow(FALSE);
	m_quality=GetDlgItem(IDC_COMBO_QUALITY);
	m_quality.EnableWindow(FALSE);
	m_quality.AddString("最好");
	m_quality.AddString("较好");
	m_quality.AddString("一般");
	m_quality.AddString("较差");
	m_quality.AddString("最差");
	m_quality_sub=GetDlgItem(IDC_COMBO_QUALITY_SUB);
	m_quality_sub.EnableWindow(FALSE);
	m_quality_sub.AddString("最好");
	m_quality_sub.AddString("较好");
	m_quality_sub.AddString("一般");
	m_quality_sub.AddString("较差");
	m_quality_sub.AddString("最差");
	m_format=GetDlgItem(IDC_COMBO_FORMAT);
	m_format.EnableWindow(FALSE);
	m_format.AddString("CIF");
	m_format.AddString("QCIF");
	m_format.AddString("2CIF");
	m_format.AddString("D1");
	m_format_sub=GetDlgItem(IDC_COMBO_FORMAT_SUB);
	m_format_sub.EnableWindow(FALSE);
	m_format_sub.AddString("QCIF");
	m_format_sub.AddString("CIF");
	m_format_sub.AddString("2CIF");
	m_format_sub.AddString("HD1");
	m_frameRate=GetDlgItem(IDC_COMBO_FRAMERATE);
	m_frameRate.EnableWindow(FALSE);
	//N 制：1,2,3,4,5,6,7,10,15,30；
	//P 制：1,2,3,4,5,6,8,12,25。
	m_frameRate.AddString("1");
	m_frameRate.AddString("2");
	m_frameRate.AddString("3");
	m_frameRate.AddString("4");
	m_frameRate.AddString("5");
	m_frameRate.AddString("6");
	m_frameRate.AddString("8");
	m_frameRate.AddString("12");
	m_frameRate.AddString("25");
	m_frameRate_sub=GetDlgItem(IDC_COMBO_FRAMERATE_SUB);
	m_frameRate_sub.EnableWindow(FALSE);
	//N 制：1,2,3,4,5,6,7,10,15,30；
	//P 制：1,2,3,4,5,6,8,12,25。
	m_frameRate_sub.AddString("1");
	m_frameRate_sub.AddString("2");
	m_frameRate_sub.AddString("3");
	m_frameRate_sub.AddString("4");
	m_frameRate_sub.AddString("5");
	m_frameRate_sub.AddString("6");
	m_frameRate_sub.AddString("8");
	m_frameRate_sub.AddString("12");
	m_frameRate_sub.AddString("25");
	m_maxBit=GetDlgItem(IDC_COMBO_MAXBIT);
	m_maxBit.EnableWindow(FALSE);
	m_maxBit.AddString("300");
	m_maxBit_sub=GetDlgItem(IDC_COMBO_MAXBIT_SUB);
	m_maxBit_sub.EnableWindow(FALSE);
	m_maxBit_sub.AddString("300");
}

LRESULT CEncodeSettingDlg::OnBnClickedCheckSub(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	checkSub();
	return 0;
}

void CEncodeSettingDlg::checkSub()
{
	if (m_captureSub.GetCheck() == false){
		m_quality_sub.EnableWindow(FALSE);
		m_format_sub.EnableWindow(FALSE);
		m_frameRate_sub.EnableWindow(FALSE);
		m_maxBit_sub.EnableWindow(FALSE);
	}else{
		m_quality_sub.EnableWindow(TRUE);
		m_format_sub.EnableWindow(TRUE);
		m_frameRate_sub.EnableWindow(TRUE);
		m_maxBit_sub.EnableWindow(TRUE);
	}
}

void CEncodeSettingDlg::saveValue()
{
	if (m_selectedChannel != -1)
	{
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].audio =m_captureAudio.GetCheck();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].sub = m_captureSub.GetCheck();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].quality =m_quality.GetCurSel();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].quality_sub = m_quality_sub.GetCurSel();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].format = m_format.GetCurSel();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].format_sub = m_format_sub.GetCurSel();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].frameRate = m_frameRate.GetCurSel();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].frameRate = m_frameRate_sub.GetCurSel();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].maxBit = m_maxBit.GetCurSel();
		m_localEncodeSetting[m_channelIndex[m_selectedChannel]].maxBit_sub = m_maxBit_sub.GetCurSel();
	}
}

LRESULT CEncodeSettingDlg::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	return 0;
}

