#include "stdafx.h"
#include "PlayDlg.h"
#include "dhplay.h"
#include "RecordManager.h"


LRESULT CPlayDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SetWindowPos(NULL,0,0,800,600,SWP_SHOWWINDOW);
	CenterWindow();
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);

	RECT rect = {224, 0, 224+352, 288};
	m_playWindow.Create(m_hWnd, rect ,NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);		
	m_timeSlide.Attach(GetDlgItem(IDC_SLIDER_TIME));
	m_timeSlide.SetRange(0,99,TRUE);
	GetDlgItem(IDC_BUTTON_PAUSE).SetWindowText(L"ÔÝÍ£");

// 	PLAY_GetFreePort(&m_port);
// 	//PLAY_OpenFile(m_port,m_filePath);
// 
// 	m_totalTime= PLAY_GetFileTime(m_port);
// 	CString totalTime;
// 	totalTime.Format(L"%dH%dM%dS",m_totalTime/3600,m_totalTime%3600/60,m_totalTime%60);
// 	SetTimer(1,100);
// 	GetDlgItem(IDC_STATIC_TOTALTIME).SetWindowText(totalTime);
// 	PLAY_Play(m_port,m_playWindow.m_hWnd);
// 	m_isPlaying = TRUE;
	
	m_calendarCtrl = GetDlgItem(IDC_MONTHCALENDAR);
	initSelectButton();
	SYSTEMTIME st;
	GetLocalTime(&st);
	return TRUE;
}

LRESULT CPlayDlg::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CPlayDlg::OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//EndDialog(0);
	DestroyWindow();
	return 0;
}

LRESULT CPlayDlg::OnTimer( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	int m_playedTimeEx = PLAY_GetPlayedTimeEx(m_port);
	m_playedTime =m_playedTimeEx/1000;
	int second = ((double)(m_playedTimeEx%60000))/1000+0.5;
	//PLAY_GetPlayPos(m_port);
	//m_timeSlide.SetPos(m_playedTime*100/m_totalTime);
	m_timeSlide.SetPos(PLAY_GetPlayPos(m_port)*100);

	CString palyedTime;
	palyedTime.Format(L"%dH%dM%dS",m_playedTime/3600,m_playedTime%3600/60,second);
	GetDlgItem(IDC_STATIC_PLAYEDTIME).SetWindowText(palyedTime);

	return TRUE;
}

LRESULT CPlayDlg::OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	double temp = m_timeSlide.GetPos();
	PLAY_SetPlayPos(m_port,temp/100);
	return 0;
}

LRESULT CPlayDlg::OnBnClickedButtonPause(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_isPlaying)
	{
		PLAY_Pause(m_port,TRUE);
		m_isPlaying = FALSE;
		GetDlgItem(IDC_BUTTON_PAUSE).SetWindowText(L"²¥·Å");
	} 
	else
	{
		PLAY_Pause(m_port,FALSE);
		m_isPlaying = TRUE;
		GetDlgItem(IDC_BUTTON_PAUSE).SetWindowText(L"ÔÝÍ£");
	}
	
	return 0;
}

LRESULT CPlayDlg::OnMcnGetdaystateMonthcalendar(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMDAYSTATE pDayState = reinterpret_cast<LPNMDAYSTATE>(pNMHDR);
	int iMax = pDayState->cDayState;

	for(int i = 0; i < iMax; i++)
	{
		LookUpbyMonth(pDayState->stStart,pDayState->prgDayState);
	}
	return 0;
}

LRESULT CPlayDlg::OnMcnSelchangeMonthcalendar(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	SYSTEMTIME st;
	m_calendarCtrl.GetCurSel(&st);
	LookUpbyDay(st,&m_recordAvailable[0][0]);
	updateSelectButton();
	return 0;
}

void CPlayDlg::initSelectButton()
{
	RECT rc ={250,400,250+20,400+20};
	for (int i= 0;i<BLM_CHANNEL_MAX;i++)
	{
		for(int j=0;j<24;j++)
		{
			m_selectButton[i][j].Create(m_hWnd, rc, L"",WS_CHILD | WS_VISIBLE);
			m_selectButton[i][j].SetWindowText(L"");
			rc.left+=20;
			rc.right+=20;
		}
		rc.left = 250;
		rc.right= 250+20;
		rc.bottom+=20;
		rc.top+=20;
	}

	updateSelectButton();
}

void CPlayDlg::updateSelectButton()
{
	for (int i = 0;i<BLM_CHANNEL_MAX;i++)
	{
		for(int j=0;j<24;j++)
		{
			m_selectButton[i][j].EnableWindow(m_recordAvailable[i][j]);
		}
	}
}
