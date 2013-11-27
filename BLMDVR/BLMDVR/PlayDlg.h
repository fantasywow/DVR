/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"
#include "BlmTypes.h"
#include "dhvecsystem.h"
#include <vector>
using namespace std;

class CPlayWindow : public CFrameWindowImpl<CPlayWindow>
{
public:
	DECLARE_WND_CLASS(NULL)
// 	BEGIN_MSG_MAP(CPlayWindow)
// 		MESSAGE_HANDLER(WM_PAINT, OnPaint)
// 		MESSAGE_HANDLER(WM_CREATE, OnCreate)
// 	END_MSG_MAP()
// 	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
// 	{
// 		CPaintDC dc(m_hWnd);
// 		//TODO: Add your drawing code here
// 		return 0;
// 	}
// 	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
// 	{
// 		return 0;
// 	}
};


class CPlayDlg : public CDialogImpl<CPlayDlg> ,
	public CUpdateUI<CPlayDlg>,
	public CMessageFilter, 
	public CIdleHandler
{
private:
	CPlayWindow m_playWindow;
	CTrackBarCtrl m_timeSlide;
	CMonthCalendarCtrl m_calendarCtrl;
	LONG m_port;
	int m_playBeginTime,m_playEndTime;
	int m_currentCalendarMonth;
	int m_recordIndex;
	BOOL m_isPlaying;
	vector<BlmRecord> m_oneDayRecord;
	int m_channel;
public:
	enum { IDD = IDD_DIALOG_PLAY };


	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CPlayDlg)
	END_UPDATE_UI_MAP()
	
	BEGIN_MSG_MAP(CPlayDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_CLOSE,OnClose)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(BM_FILE_END, OnFileEnd)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_HANDLER(IDC_BUTTON_PAUSE, BN_CLICKED, OnBnClickedButtonPause)
		NOTIFY_HANDLER(IDC_MONTHCALENDAR, MCN_GETDAYSTATE, OnMcnGetdaystateMonthcalendar)
		NOTIFY_HANDLER(IDC_MONTHCALENDAR, MCN_SELCHANGE, OnMcnSelchangeMonthcalendar)
	END_MSG_MAP()

	LRESULT OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonPause(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMcnGetdaystateMonthcalendar(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnMcnSelchangeMonthcalendar(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnFileEnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void showOneDayRecord();
	void checkPos( int pos );
	void calCurrentPos();
	bool playFile(CString fileName,int beginTime,int endTime);
	void closeFile();
};
