#include "stdafx.h"
#include "PlayDlg.h"
#include "dhplay.h"
#include "RecordManager.h"
#include "MainDlg.h"
//100 zhuan 60
inline double convert100to60(int arg){
	int	hour = arg/10000;
	int minute = arg/100%100;
	int second = arg%100;
	return hour*3600+minute*60+second;
}



LRESULT CPlayDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	m_isPlaying = FALSE;
	m_port = -1;
	m_channelID = 0;
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

	m_channelList.Attach(GetDlgItem(IDC_LIST_CHANNEL));
	for (int i =0 ;i<BLM_CHANNEL_MAX;i++)
	{
		m_channelList.AddString(m_parent->m_settingDlg->m_channelName[i]);
	}
	m_channelList.SetCurSel(0);


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
	checkPos(pos);
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
				if (m_oneDayRecord[i].channel==m_channelID)
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
	closeFile();
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
	for (int i =0; i<m_recordIcon.size();i++)
	{
		m_recordIcon[i]->DestroyWindow();
	}
	m_recordIcon.clear();
	
	
	for(int i=0;i<m_oneDayRecord.size();i++){
		//250 400
		CStatic *record = new CStatic();
		int begin,end;
		begin = 550 * convert100to60(m_oneDayRecord[i].beginTime)/(24*60*60);
		end = 550 * convert100to60(m_oneDayRecord[i].endTime)/(24*60*60);
		record->Create(m_hWnd, CRect(0,0,end-begin,15) ,NULL, WS_CHILD | WS_VISIBLE|SS_BLACKRECT, NULL);		
		record->SetWindowPos(NULL,250+begin,400+30*m_oneDayRecord[i].channel,end-begin,15,SWP_SHOWWINDOW);
		m_recordIcon.push_back(record);
	}
	
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
		if (pos>=m_oneDayRecord[i].beginTime&&pos<=m_oneDayRecord[i].endTime&&m_oneDayRecord[i].channel==m_channelID)
		{
			closeFile();
			playFile(m_oneDayRecord[i].fileName,m_oneDayRecord[i].beginTime,m_oneDayRecord[i].endTime);
			float filepos = float(pos-convert100to60(m_oneDayRecord[i].beginTime))/(convert100to60(m_oneDayRecord[i].beginTime)-convert100to60(m_oneDayRecord[i].endTime));
			PLAY_SetPlayPos(m_port,filepos);
			return;
		}
	}
	//检查最近文件
	for(int i =0 ;i<m_oneDayRecord.size();i++){
		if (pos<m_oneDayRecord[i].beginTime&&m_oneDayRecord[i].channel==m_channelID)
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
		if (m_oneDayRecord[i].beginTime>m_playEndTime&&m_oneDayRecord[i].channel==m_channelID)
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
	if (PLAY_OpenFile(m_port,fileName.GetBuffer(0))==FALSE)
	{
		missingOneRecord(fileName);
		m_port = -1;
		return false;
	}
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


LRESULT CPlayDlg::OnLbnSelchangeListChannel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_channelID =  m_channelList.GetCurSel();
	closeFile();
	m_timeSlide.SetPos(0);
	return 0;
}
