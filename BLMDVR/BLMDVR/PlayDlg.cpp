#include "stdafx.h"
#include "PlayDlg.h"
#include "dhplay.h"
#include "RecordManager.h"

//100 zhuan 60
int convert100to60(int arg){
	int	hour = arg/10000;
	int minute = arg/100%100;
	int second = arg%100;
	return hour*3600+minute*60+second;
}



LRESULT CPlayDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	m_isPlaying = FALSE;
	m_port = -1;
	m_channel = 0;
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
	m_timeSlide.SetRangeMax(24*60*60);
	m_timeSlide.SetPageSize(1);	
	GetDlgItem(IDC_BUTTON_PAUSE).SetWindowText("播放");

	SetTimer(1,100);
	
	m_calendarCtrl = GetDlgItem(IDC_MONTHCALENDAR);
	//初始显示今天的录像内容
	SYSTEMTIME st;
	GetLocalTime(&st);
	LookUpbyDay(st,m_oneDayRecord);
	showOneDayRecord();

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
	DestroyWindow();
	return 0;
}

LRESULT CPlayDlg::OnTimer( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	calCurrentPos();
	return TRUE;
}

LRESULT CPlayDlg::OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	int pos = m_timeSlide.GetPos();
	//checkPos(pos);
	return 0;
}

LRESULT CPlayDlg::OnBnClickedButtonPause(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_isPlaying)
	{
		PLAY_Pause(m_port,TRUE);
		m_isPlaying = FALSE;
		GetDlgItem(IDC_BUTTON_PAUSE).SetWindowText("播放");
	} 
	else
	{
		if (m_port==-1)
		{
			for(int i=0;i<m_oneDayRecord.size();i++){
				if (m_oneDayRecord[i].channel==m_channel)
				{
					bool result = playFile(m_oneDayRecord[i].fileName,m_oneDayRecord[i].beginTime,m_oneDayRecord[i].endTime);
					if (result == false)
					{
						MessageBox("文件无法打开");
					}
					return 0;
				}
			}
		}else{
			PLAY_Pause(m_port,FALSE);
			m_isPlaying = TRUE;
			GetDlgItem(IDC_BUTTON_PAUSE).SetWindowText("暂停");
		}
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
	m_timeSlide.SetPos(0);
	closeFile();
	LookUpbyDay(st,m_oneDayRecord);
	showOneDayRecord();
	return 0;
}

void CPlayDlg::showOneDayRecord()
{

}
//拖动滚动条判断函数
void CPlayDlg::checkPos( int pos )
{
	//检查当前文件
	if(pos>=m_playBeginTime&&pos<=m_playEndTime&&m_port!=-1)
	{
		float filepos = float(pos-convert100to60(m_playBeginTime))/(convert100to60(m_playEndTime)-convert100to60(m_playBeginTime));
		PLAY_SetPlayPos(m_port,filepos);
		return;
	}
	//检查是不是在其他可用文件中
	for(int i =0 ;i<m_oneDayRecord.size();i++){
		if (pos>=m_oneDayRecord[i].beginTime&&pos<=m_oneDayRecord[i].endTime&&m_oneDayRecord[i].channel==m_channel)
		{
			closeFile();
			playFile(m_oneDayRecord[i].fileName,m_oneDayRecord[i].beginTime,m_oneDayRecord[i].endTime);
			float filepos = float(pos-convert100to60(m_oneDayRecord[i].beginTime))/(convert100to60(m_oneDayRecord[i].beginTime)-convert100to60(m_oneDayRecord[i].endTime));
			PLAY_SetPlayPos(m_port,filepos);
			return;
		}
	}
	for(int i =0 ;i<m_oneDayRecord.size();i++){
		if (pos<m_oneDayRecord[i].beginTime&&m_oneDayRecord[i].channel==m_channel)
		{
			closeFile();
			playFile(m_oneDayRecord[i].fileName,m_oneDayRecord[i].beginTime,m_oneDayRecord[i].endTime);
			m_timeSlide.SetPos(convert100to60(m_oneDayRecord[i].beginTime));
			return;
		}
	}
	m_timeSlide.SetPos(0);

}

LRESULT CPlayDlg::OnFileEnd( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	//find next file and play
	for(int i =0 ;i<m_oneDayRecord.size();i++){
		if (m_oneDayRecord[i].beginTime>m_playEndTime&&m_oneDayRecord[i].channel==m_channel)
		{
			closeFile();
			playFile(m_oneDayRecord[i].fileName,m_oneDayRecord[i].beginTime,m_oneDayRecord[i].endTime);
			m_timeSlide.SetPos(convert100to60(m_oneDayRecord[i].beginTime));
		}
	}
	return 0;
}

void CPlayDlg::calCurrentPos()
{
	if (m_port!=-1)
	{
		float filePos= 0; //for test
		//float filePos = PLAY_GetPlayPos(m_port);
		int pos = m_playBeginTime+(m_playEndTime-m_playBeginTime)*filePos;
		m_timeSlide.SetPos(convert100to60(pos));
	}
}

bool CPlayDlg::playFile( CString fileName,int beginTime,int endTime )
{
	PLAY_GetFreePort(&m_port);
	/*
	if (PLAY_OpenFile(m_port,fileName.GetBuffer(0))==FALSE);
	{
		missingOneRecord(fileName);
		m_port = -1;
		return false;
	}
	*/
	PLAY_Play(m_port,m_playWindow.m_hWnd);
	m_isPlaying = TRUE;
	PLAY_SetFileEndMsg(m_port,m_hWnd,BM_FILE_END);
	m_playBeginTime = beginTime;
	m_playEndTime   = endTime;
	return true;
}

void CPlayDlg::closeFile()
{
	if (m_port!=-1)
	{
		PLAY_Stop(m_port);
		PLAY_CloseFile(m_port);
	}
	m_port = -1;
}

