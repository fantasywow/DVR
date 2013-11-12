#include "stdafx.h"
#include "SettingDlg.h"


CSettingDlg::CSettingDlg( HANDLE * phChannel )
{
	m_channelHandle = phChannel;
	for (int i=0;i<BLM_CHANNEL_MAX;i++)
	{
		m_channelName[i].Format(L"Camera%d",i);
	}
	m_capturePath = L"C:\\";
	m_encodeDlg = new CEncodeSettingDlg(this);
	m_pictureDlg= new CPictureSettingDlg(this);
	m_basicDlg  = new CBasicSettingDlg(this);
	m_recodeDlg = new CRecordSettingDlg(this);
	m_cradleDlg = new CCradleSettingDlg(this);
}


LRESULT CSettingDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){
	SetWindowPos(NULL,0,0,800,500,SWP_SHOWWINDOW);
	CenterWindow();


	m_basicDlg->Create(m_hWnd);
	m_encodeDlg->Create(m_hWnd);
	m_pictureDlg->Create(m_hWnd);
	m_recodeDlg->Create(m_hWnd);
	m_cradleDlg->Create(m_hWnd);
	m_basicDlg->SetWindowPos(NULL,100,0,700,400,SWP_SHOWWINDOW);
	m_encodeDlg->SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	m_pictureDlg->SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	m_recodeDlg->SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	m_cradleDlg->SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	
	return true;
}


LRESULT CSettingDlg::OnBnClickedButtonBasic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg->ShowWindow(SW_SHOW);
	m_encodeDlg->ShowWindow(SW_HIDE);
	m_pictureDlg->ShowWindow(SW_HIDE);
	m_recodeDlg->ShowWindow(SW_HIDE);
	m_cradleDlg->ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonPicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg->ShowWindow(SW_HIDE);
	m_encodeDlg->ShowWindow(SW_HIDE);
	m_pictureDlg->ShowWindow(SW_SHOW);
	m_recodeDlg->ShowWindow(SW_HIDE);
	m_cradleDlg->ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonEncode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg->ShowWindow(SW_HIDE);
	m_encodeDlg->ShowWindow(SW_SHOW);
	m_pictureDlg->ShowWindow(SW_HIDE);
	m_recodeDlg->ShowWindow(SW_HIDE);
	m_cradleDlg->ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonCradle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg->ShowWindow(SW_HIDE);
	m_encodeDlg->ShowWindow(SW_HIDE);
	m_pictureDlg->ShowWindow(SW_HIDE);
	m_recodeDlg->ShowWindow(SW_HIDE);
	m_cradleDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonRecode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg->ShowWindow(SW_HIDE);
	m_encodeDlg->ShowWindow(SW_HIDE);
	m_pictureDlg->ShowWindow(SW_HIDE);
	m_recodeDlg->ShowWindow(SW_SHOW);
	m_cradleDlg->ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedConfirm(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 添加保存设置的功能
	m_basicDlg->SendMessage(BM_CONFIRM_SETTING);
	m_encodeDlg->SendMessage(BM_CONFIRM_SETTING);
	m_pictureDlg->SendMessage(BM_CONFIRM_SETTING);
	m_recodeDlg->SendMessage(BM_CONFIRM_SETTING);
	m_cradleDlg->SendMessage(BM_CONFIRM_SETTING);
	EndDialog(wID);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


void CSettingDlg::OSD( int channelId,bool Name,bool Time )
{
	if (m_channelHandle[channelId]!=INVALID_HANDLE_VALUE)
	{
		USHORT OSDTime[] = {0x00, 0x00, _OSD_YEAR4, '-',_OSD_MONTH3,'-',_OSD_DAY,'-',  _OSD_HOUR12, ':', _OSD_MINUTE,':', _OSD_SECOND, '\0'};
		USHORT OSDName[] = {0x60, 0x00,'C','h',' ',' ', ' ', 'O', 'S', 'D',' ', 'E', 'X', 'A', 'M', 'P', 'L', 'E',0};
		USHORT *Format[2];
		Format[0]=OSDTime;
		Format[1]=OSDName;
		SetOsdDisplayModeEx(m_channelHandle[channelId], 255, FALSE, 1, 2, Format);
		int iRet = SetOsd(m_channelHandle[channelId], TRUE);
		if (iRet != SS_SUCCESS)
		{
			MessageBox(L"OSD FAILED");
		}
	}
}

LRESULT CSettingDlg::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	return 0;
}

LRESULT CSettingDlg::OnClose( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	EndDialog(0);
	return TRUE;
}
