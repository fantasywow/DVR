// MainDlg.h : interface of the CBlmCalDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define SKIN_LOAD_FROM_FOLDER
#define _SKIN_FILE_DIR _T("ControlSkin")

#define IDC_BLMCAL_YEAR_LEFT  8000
#define IDC_BLMCAL_YEAR_RIGHT 8001
#define IDC_BLMCAL_MONTH_LEFT 8002
#define IDC_BLMCAL_MONTH_RIGHT 8003
#define IDC_BLMCAL_YEAR  8004
#define IDC_BLMCAL_MONTH 8005

#define IDC_BLMCAL_WEEK1  8006
#define IDC_BLMCAL_WEEK2 8007
#define IDC_BLMCAL_WEEK3  8008
#define IDC_BLMCAL_WEEK4 8009
#define IDC_BLMCAL_WEEK5  8010
#define IDC_BLMCAL_WEEK6 8011
#define IDC_BLMCAL_WEEK7  8012

#define IDC_BLMCAL_HOUR_UP  8013
#define IDC_BLMCAL_HOUR_DOWN 8014
#define IDC_BLMCAL_MIN_UP 8015
#define IDC_BLMCAL_MIN_DOWN 8016

#define IDC_BLMCAL_HOUR_STATIC  8017
#define IDC_BLMCAL_MIN_STATIC 8018
#define IDC_BLMCAL_HOUR_EDIT 8019
#define IDC_BLMCAL_MIN_EDIT 8020

#define IDC_BLMCAL_OK 8021
#define IDC_BLMCAL_CANCEL 8022

#define CAL_BUTTONID 8023
#define CAL_BUTTONNUM 42

#define BTN_WIDTH 32
#define BTN_HEIGTH 32
#define BTN_SPLIT  5


//#define UM_CHANGEDATE WM_USER+1
#define UM_CHANGEDATETIME WM_USER+2


#include <vector>
#include "resource.h"

#include "Dtz.h"
#include "StringUtl.h"
#include "BlmKernel.h"


using namespace Boloomo::Util;
using std::vector;


static CString WEEKSTR_EN[]={_T("Sun"),_T("Mon"),_T("Tue"),_T("Wed"),_T("Thu"),_T("Fri"),_T("Sat")};
static CString WEEKSTR_CN[]={_T("日"),_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六")};
static CString MONTHSTR_EN[]={_T("January"),_T("February"),_T("March"),_T("April"),_T("May"),_T("June"),_T("July"),_T("August"),_T("September"),_T("October"),_T("November"),_T("December")};
static CString MONTHSTR_CN[]={_T("1月"),_T("2月"),_T("3月"),_T("4月"),_T("5月"),_T("6月"),_T("7月"),_T("8月"),_T("9月"),_T("10月"),_T("11月"),_T("12月")};

#define SETCTRLVALUE_STR(CTRLID,valuestr){ LPCTSTR PT_STR = (LPCTSTR) StringUtl::TranslateString(valuestr);\
	GetDlgItem(CTRLID).SetWindowText(PT_STR);  delete PT_STR; }

#define SETCTRLVALUE_INT(CTRLID,valueint){ LPCTSTR PT_STR = (LPCTSTR) StringUtl::TranslateString(StringUtl::IntToString(valueint));\
	GetDlgItem(CTRLID).SetWindowText(PT_STR);  delete PT_STR; }

class CBlmCalDlg : public CDialogImpl<CBlmCalDlg>, public CUpdateUI<CBlmCalDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_BLMCALDLG };


	BEGIN_UPDATE_UI_MAP(CBlmCalDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CBlmCalDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ACTIVATE,OnLoseFocus)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_HANDLER(IDC_BLMCAL_OK,BN_CLICKED,OnOK)
		COMMAND_ID_HANDLER(IDC_BLMCAL_CANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_RANGE_CODE_HANDLER(IDC_BLMCAL_YEAR_LEFT,IDC_BLMCAL_MONTH_RIGHT,BN_CLICKED,OnChangeYearMonth)
		COMMAND_RANGE_CODE_HANDLER(CAL_BUTTONID,CAL_BUTTONID+CAL_BUTTONNUM-1,BN_CLICKED,OnSelectDay)
		COMMAND_RANGE_CODE_HANDLER(CAL_BUTTONID,CAL_BUTTONID+CAL_BUTTONNUM-1,BN_DBLCLK,OnSelectDayTime)
		COMMAND_HANDLER(IDC_BLMCAL_HOUR_STATIC,STN_CLICKED,OnSelectHour)
		COMMAND_HANDLER(IDC_BLMCAL_MIN_STATIC,STN_CLICKED,OnSelectMin)
		COMMAND_RANGE_CODE_HANDLER(IDC_BLMCAL_HOUR_UP,IDC_BLMCAL_MIN_DOWN,BN_CLICKED,OnChangeHourMin)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)


public:

	CBlmCalDlg::CBlmCalDlg():m_bDateOrTime(false),
		m_ndaysBegin(0),
		m_ndaysEnd(0),
		m_nChecked(0),
		m_bMin(false),
		m_bMax(false)
	{

	}
	~CBlmCalDlg()
	{
		for (int i=0;i<m_vecButton.size();i++)
		{
			if (m_vecButton[i])
			{
				if (m_vecButton[i]->IsWindow())
				{
					m_vecButton[i]->DestroyWindow();
				}
				delete m_vecButton[i];
			}
		}
		m_vecButton.clear();

		for (int i=0;i<m_vecChooseymBtn.size();i++)
		{
			if (m_vecChooseymBtn[i])
			{
				if (m_vecChooseymBtn[i]->IsWindow())
				{
					m_vecChooseymBtn[i]->DestroyWindow();
				}
				delete m_vecChooseymBtn[i];
			}
		}
		m_vecChooseymBtn.clear();

		for (int i=0;i<m_vecYearMonthStatic.size();i++)
		{
			if (m_vecYearMonthStatic[i])
			{
				if (m_vecYearMonthStatic[i]->IsWindow())
				{
					m_vecYearMonthStatic[i]->DestroyWindow();
				}
				delete m_vecYearMonthStatic[i];
			}
		}
		m_vecYearMonthStatic.clear();

		for (int i=0;i<m_vecWeekStatic.size();i++)
		{
			if (m_vecWeekStatic[i])
			{
				if (m_vecWeekStatic[i]->IsWindow())
				{
					m_vecWeekStatic[i]->DestroyWindow();
				}
				delete m_vecWeekStatic[i];
			}
		}
		m_vecWeekStatic.clear();

		for (int i=0;i<m_vecChoosehourBtn.size();i++)
		{
			if (m_vecChoosehourBtn[i])
			{
				if (m_vecChoosehourBtn[i]->IsWindow())
				{
					m_vecChoosehourBtn[i]->DestroyWindow();
				}
				delete m_vecChoosehourBtn[i];
			}
		}
		m_vecChoosehourBtn.clear();

		for (int i=0;i<m_vecChooseminBtn.size();i++)
		{
			if (m_vecChooseminBtn[i])
			{
				if (m_vecChooseminBtn[i]->IsWindow())
				{
					m_vecChooseminBtn[i]->DestroyWindow();
				}
				delete m_vecChooseminBtn[i];
			}
		}
		m_vecChooseminBtn.clear();

		for (int i=0;i<m_vecTimeStatic.size();i++)
		{
			if (m_vecTimeStatic[i])
			{
				if (m_vecTimeStatic[i]->IsWindow())
				{
					m_vecTimeStatic[i]->DestroyWindow();
				}
				delete m_vecTimeStatic[i];
			}
		}
		m_vecTimeStatic.clear();

		for (int i=0;i<m_vecTimeEdit.size();i++)
		{
			if (m_vecTimeEdit[i])
			{
				if (m_vecTimeEdit[i]->IsWindow())
				{
					m_vecTimeEdit[i]->DestroyWindow();
				}
				delete m_vecTimeEdit[i];
			}
		}
		m_vecTimeEdit.clear();

		for (int i=0;i<m_vecOkCancelBtn.size();i++)
		{
			if (m_vecOkCancelBtn[i])
			{
				if (m_vecOkCancelBtn[i]->IsWindow())
				{
					m_vecOkCancelBtn[i]->DestroyWindow();
				}
				delete m_vecOkCancelBtn[i];
			}
		}
		m_vecOkCancelBtn.clear();
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (IsWindow() && IsWindowVisible())
		{
			if (pMsg->message == WM_LBUTTONDOWN)
			{
				if (((GetFocus()==GetDlgItem(IDC_BLMCAL_HOUR_EDIT) && pMsg->hwnd!=GetDlgItem(IDC_BLMCAL_HOUR_EDIT))
					|| (GetFocus()!=GetDlgItem(IDC_BLMCAL_HOUR_EDIT) && ::IsWindowVisible(GetDlgItem(IDC_BLMCAL_HOUR_EDIT))))&&
					(pMsg->hwnd!=GetDlgItem(IDC_BLMCAL_HOUR_UP) && pMsg->hwnd!=GetDlgItem(IDC_BLMCAL_HOUR_DOWN)))
				{
					_ChangeHourMinEdit(true,false);
				}
				else if (((GetFocus()==GetDlgItem(IDC_BLMCAL_MIN_EDIT) && pMsg->hwnd!=GetDlgItem(IDC_BLMCAL_MIN_EDIT))
					|| (GetFocus()!=GetDlgItem(IDC_BLMCAL_MIN_EDIT) && ::IsWindowVisible(GetDlgItem(IDC_BLMCAL_MIN_EDIT))))&&
					(pMsg->hwnd!=GetDlgItem(IDC_BLMCAL_MIN_UP) && pMsg->hwnd!=GetDlgItem(IDC_BLMCAL_MIN_DOWN)))
				{
					_ChangeHourMinEdit(false,false);
				}
				if (pMsg->hwnd != m_hWnd && !::IsChild(m_hWnd,pMsg->hwnd)&& !::IsChild(m_parentMsgWnd,pMsg->hwnd) )
				{
					if(IsWindow() && IsWindowVisible())
						ShowWindow(SW_HIDE);
				}
			}
			else if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
			{
				if (GetFocus() == GetDlgItem(IDC_BLMCAL_HOUR_EDIT))
					_ChangeHourMinEdit(true,false);
				else if (GetFocus() == GetDlgItem(IDC_BLMCAL_MIN_EDIT))
					_ChangeHourMinEdit(false,false);
			}
		}
		
		return CWindow::IsDialogMessage(pMsg);
	}

	BOOL OnIdle()
	{
		return FALSE;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

// 		// set icons
// 		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
// 			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
// 		SetIcon(hIcon, TRUE);
// 		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
// 			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
// 		SetIcon(hIconSmall, FALSE);

// 		// register object for message filtering and idle updates
// 		CMessageLoop* pLoop = _Module.GetMessageLoop();
// 		ATLASSERT(pLoop != NULL);
// 		pLoop->AddMessageFilter(this);
// 		pLoop->AddIdleHandler(this);
		CAppModule* _pModule = BlmKernel::AppModule();
		if(_pModule)
		{
			CMessageLoop* pLoop = _pModule->GetMessageLoop();
			ATLASSERT(pLoop != NULL);
			pLoop->AddMessageFilter(this);
			pLoop->AddIdleHandler(this);
		}

		UIAddChildWindowContainer(m_hWnd);
	
		dsSkinWindow(m_hWnd, SKIN_TYPE_DIALOG, _T("Blm_DateDlg"), FALSE);
		if (!m_bDateOrTime)
		{
			CRect rc;
			GetWindowRect(&rc);
			rc.bottom +=69;
			MoveWindow(&rc);
			dsSetDrawItemValue( m_hWnd, _T("datebgimage"), _T("DateTimeDlg"),TRUE);
		}

		_CreateView();

		//m_showdt = _datetime_t::now();
		_CreateCal(m_showdt);

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
// 		// unregister message filtering and idle updates
// 		CMessageLoop* pLoop = _Module.GetMessageLoop();
// 		ATLASSERT(pLoop != NULL);
// 		pLoop->RemoveMessageFilter(this);
// 		pLoop->RemoveIdleHandler(this);
		CAppModule* _pModule = BlmKernel::AppModule();
		if(_pModule)
		{
			CMessageLoop* pLoop = _pModule->GetMessageLoop();
			ATLASSERT(pLoop != NULL);
			pLoop->RemoveMessageFilter(this);
			pLoop->RemoveIdleHandler(this);
		}

		return 0;
	}

	LRESULT OnLoseFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(this->IsWindow())
		{
			if(this->IsWindowVisible())
			{
				ShowWindow(SW_HIDE);
			}
		}
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		//仅当显示日期与时间时有效
		if (!m_bDateOrTime)
		{
			ShowWindow(SW_HIDE);

			//通知父窗口时间改变
			long selectdays = m_ndaysBegin + m_nChecked;
			_time_t tt = m_showdt.time();
			tt.set_hour(_GetMinHourVal(IDC_BLMCAL_HOUR_STATIC));
			tt.set_min(_GetMinHourVal(IDC_BLMCAL_MIN_STATIC));
			_datetime_t  dtt;
			dtt.from_csecs_days(tt.to_csecs(),selectdays);

			if (dtt == m_showdt)
			{
			}
			else
			{
				//向父窗口发送时间改变信息
				//SendMessage(GetParent(),UM_CHANGEDATETIME,selectdays,dtt.to_csecs());
				m_showdt = dtt;
				SendMessage(m_parentMsgWnd,UM_CHANGEDATETIME,(WPARAM)&dtt,(LPARAM)m_hWnd);

				//DestroyWindow();
			}
		}

		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//CloseDialog(wID);
		ShowWindow(SW_HIDE);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

	LRESULT OnChangeYearMonth(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		switch (wID)
		{
		case IDC_BLMCAL_YEAR_LEFT:
			{
				_datetime_t dtt = m_showingdt;
				dtt.set_day(1);
				dtt.set_year(m_showingdt.year()-1);

				_CreateCal(dtt);
			}
			break;
		case IDC_BLMCAL_YEAR_RIGHT:
			{
				_datetime_t dtt = m_showingdt;
				dtt.set_day(1);
				dtt.set_year(m_showingdt.year()+1);

				_CreateCal(dtt);
			}
			break;
		case IDC_BLMCAL_MONTH_LEFT:
			{
				_datetime_t dtt = m_showingdt;
				dtt.set_day(1);
				if (m_showingdt.month() == 1)
				{
					dtt.set_year(m_showingdt.year()-1);
					dtt.set_month(12);
				}
				else
				{
					dtt.set_month(m_showingdt.month()-1);
				}
				_CreateCal(dtt);
			}
			break;
		case IDC_BLMCAL_MONTH_RIGHT:
			{
				_datetime_t dtt = m_showingdt;
				dtt.set_day(1);
				if (m_showingdt.month() == 12)
				{
					dtt.set_year(m_showingdt.year()+1);
					dtt.set_month(1);
				}
				else
				{
					dtt.set_month(m_showingdt.month()+1);
				}
				_CreateCal(dtt);
			}
			break;
		default:
			break;
		}
		return 0;
	}

	LRESULT OnSelectDay(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//如果只显示日期，单击某天时就是选中这天，并隐藏对话框
		//如果显示日期和时间，单击不做任何处理，只需要记录下当前选中第几个，需要其点击OK按钮才行
		if (m_bDateOrTime)
		{
			ShowWindow(SW_HIDE);
			//通知父窗口时间改变
			long selectdays = m_ndaysBegin + wID - CAL_BUTTONID;
			long indays = m_showdt.to_days();

			if (selectdays != indays)
			{
				_datetime_t dtt;
				dtt.from_days(selectdays);
				//向父窗口发送时间改变信息
				m_showdt = dtt;
				SendMessage(m_parentMsgWnd,UM_CHANGEDATETIME,(WPARAM)&dtt,(LPARAM)m_hWnd);

				//DestroyWindow();
			}
		}
		else
		{
			m_nChecked = wID - CAL_BUTTONID;
		}
		return 0;
	}


	LRESULT OnSelectDayTime(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//当显示显示日期和时间，双击选中
		if (!m_bDateOrTime)
		{
			m_nChecked = wID - CAL_BUTTONID;

			ShowWindow(SW_HIDE);
			//通知父窗口时间改变
			long selectdays = m_ndaysBegin + m_nChecked;
			_time_t tt = m_showdt.time();
			tt.set_hour(_GetMinHourVal(IDC_BLMCAL_HOUR_STATIC));
			tt.set_min(_GetMinHourVal(IDC_BLMCAL_MIN_STATIC));
			_datetime_t  dtt;
			dtt.from_csecs_days(tt.to_csecs(),selectdays);

			if (dtt == m_showdt)
			{
			}
			else
			{
				//向父窗口发送时间改变信息
				//SendMessage(GetParent(),UM_CHANGEDATETIME,selectdays,dtt.to_csecs());
				m_showdt = dtt;
				SendMessage(m_parentMsgWnd,UM_CHANGEDATETIME,(WPARAM)&dtt,(LPARAM)m_hWnd);
			}
		}
		return 0;
	}

	LRESULT OnSelectHour(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		_ChangeHourMinEdit(true,true);
		return 0;
	}

	LRESULT OnSelectMin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		_ChangeHourMinEdit(false,true);
		return 0;
	}

	LRESULT OnChangeHourMin(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		switch (wID)
		{
		case IDC_BLMCAL_HOUR_UP:
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_HOUR_EDIT).GetWindowText(str);
				int ntemp = _ttoi(str);

				ntemp--;
				if(ntemp == -1)
					ntemp = 23;

				_SetMinHourVal(IDC_BLMCAL_HOUR_EDIT,ntemp);

				m_vecTimeEdit[0]->SetSelAll();
				m_vecTimeEdit[0]->SetFocus();

			}
			break;
		case IDC_BLMCAL_HOUR_DOWN:
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_HOUR_EDIT).GetWindowText(str);
				int ntemp = _ttoi(str);

				ntemp++;
				if(ntemp == 24)
					ntemp = 0;

				_SetMinHourVal(IDC_BLMCAL_HOUR_EDIT,ntemp);

				m_vecTimeEdit[0]->SetSelAll();
				m_vecTimeEdit[0]->SetFocus();
			}
			break;
		case IDC_BLMCAL_MIN_UP:
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_MIN_EDIT).GetWindowText(str);
				int ntemp = _ttoi(str);

				ntemp--;
				if(ntemp == -1)
					ntemp = 59;

				_SetMinHourVal(IDC_BLMCAL_MIN_EDIT,ntemp);

				m_vecTimeEdit[1]->SetSelAll();
				m_vecTimeEdit[1]->SetFocus();
			}
			break;
		case IDC_BLMCAL_MIN_DOWN:
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_MIN_EDIT).GetWindowText(str);
				int ntemp = _ttoi(str);

				ntemp++;
				if(ntemp == 60)
					ntemp = 0;

				_SetMinHourVal(IDC_BLMCAL_MIN_EDIT,ntemp);

				m_vecTimeEdit[1]->SetSelAll();
				m_vecTimeEdit[1]->SetFocus();
			}
			break;
		default:
			break;
		}
		return 0;
	}

	void SetShowDate(bool bshowdateortime,_datetime_t& dtt)
	{
		m_bDateOrTime = bshowdateortime;
		m_showdt = dtt;
	}
	
	void ResetShowDateView(_datetime_t& dtt)
	{
		m_showdt = dtt;
		_CreateCal(dtt);
	}

	void SetMsgParent(HWND parentHwnd)
	{
		m_parentMsgWnd = parentHwnd;
	}

	void SetRange(bool bmin,_datetime_t mindt,bool bmax,_datetime_t maxdt)
	{
		m_bMin = bmin;
		if (m_bMin)
		{
			m_dtMin = mindt;
		}

		m_bMax = bmax;
		if (m_bMax)
		{
			m_dtMax = maxdt;
		}
	}
protected:
private:
	

	void _ChangeHourMinEdit(bool bhour,bool bedit)
	{
		if (bhour)
		{
			if (bedit)
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_HOUR_STATIC).GetWindowText(str);
				GetDlgItem(IDC_BLMCAL_HOUR_EDIT).SetWindowText(str);

				GetDlgItem(IDC_BLMCAL_HOUR_STATIC).ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BLMCAL_HOUR_EDIT).ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BLMCAL_HOUR_UP).ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BLMCAL_HOUR_DOWN).ShowWindow(SW_SHOW);
				m_vecTimeEdit[0]->SetSelAll();
				//m_vecTimeEdit[0]->SetSelNone();
				m_vecTimeEdit[0]->SetFocus();
			}
			else
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_HOUR_EDIT).GetWindowText(str);

				int ntmp = _ttoi(str);
				if (ntmp>=0 && ntmp<24)
				{
					_SetMinHourVal(IDC_BLMCAL_HOUR_STATIC,ntmp);
				}

				GetDlgItem(IDC_BLMCAL_HOUR_STATIC).ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BLMCAL_HOUR_EDIT).ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BLMCAL_HOUR_UP).ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BLMCAL_HOUR_DOWN).ShowWindow(SW_HIDE);
			}
		}
		else
		{

			if (bedit)
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_MIN_STATIC).GetWindowText(str);
				GetDlgItem(IDC_BLMCAL_MIN_EDIT).SetWindowText(str);



				GetDlgItem(IDC_BLMCAL_MIN_STATIC).ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BLMCAL_MIN_EDIT).ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BLMCAL_MIN_UP).ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BLMCAL_MIN_DOWN).ShowWindow(SW_SHOW);
				m_vecTimeEdit[1]->SetSelAll();
				//m_vecTimeEdit[1]->SetSelNone();
				m_vecTimeEdit[1]->SetFocus();
			}
			else
			{
				CString str;
				GetDlgItem(IDC_BLMCAL_MIN_EDIT).GetWindowText(str);
				int ntmp = _ttoi(str);
				if (ntmp>=0 && ntmp<60)
				{
					_SetMinHourVal(IDC_BLMCAL_MIN_STATIC,ntmp);
				}

				GetDlgItem(IDC_BLMCAL_MIN_STATIC).ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BLMCAL_MIN_EDIT).ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BLMCAL_MIN_UP).ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BLMCAL_MIN_DOWN).ShowWindow(SW_HIDE);
			}
		}
	}

	void _CreateCal(_datetime_t& show_dtt)
	{
		// 	for (int i=0;i<m_vecButton.size();i++)
		// 	{
		// 		if (m_vecButton[i]->IsWindow())
		// 		{
		// 			m_vecButton[i]->DestroyWindow();
		// 		}
		// 		delete m_vecButton[i];
		// 		m_vecButton[i] = NULL;
		// 	}
		// 	m_vecButton.clear();

		m_showingdt = show_dtt;
		int nmonthdays = show_dtt.month_days(show_dtt.month(),show_dtt.year());

		_datetime_t curr_dtt = _datetime_t::now();
		//int ndays = curr_dtt.month_days(curr_dtt.month(),curr_dtt.year());
		long days_curr = curr_dtt.to_days();//当前日期所属的天

		long days_show = show_dtt.to_days(); //当前显示日期的选中天

		long days_min = 0,days_max=0;
		if(m_bMin)
			days_min = m_dtMin.to_days();
		if (m_bMax)
			days_max = m_dtMax.to_days();


		_datetime_t dtt_begin = show_dtt;
		dtt_begin.set_day(1);
		long days_begin_month = dtt_begin.to_days();//当前显示日期所属月的第一天

		_datetime_t dtt_end = show_dtt;
		dtt_end.set_day(nmonthdays);
		long days_end_month = dtt_end.to_days();    //当前显示日期所属月的最后一天

		int ndayofweek_begin = dtt_begin.day_of_week();
		if (ndayofweek_begin == 0)
			ndayofweek_begin = 7;
		long days_begin = days_begin_month - ndayofweek_begin;  //日历显示的第一天
		m_ndaysBegin = days_begin;

		//int ndayofweek_end = dtt_end.day_of_week();
		//long days_end = days_end_month + 7 - ndayofweek_end - 1;//日历显示的最后一天
		long days_end = days_begin + CAL_BUTTONNUM-1;  //修改改默认显示42天
		m_ndaysEnd = days_end;

		int num = 0;
		int ncurr = -1;
		int nshow = -1;

		//显示本页日历上个月的那几天
		for (long premonthday = days_begin; premonthday<days_begin_month; premonthday++,num++)
		{
			_date_t dt;
			dt.from_days(premonthday);
			//CButton* pbtn = _CreateButton(num);

			CString str1;
			str1.Format(_T("DB_%d_nor"),dt.day());
			CString str2;
			str2.Format(_T("DB_%d_over"),dt.day());
			CString str3;
			str3.Format(_T("DB_%d_down"),dt.day());
			CString str4;
			str4.Format(_T("DB_%d_check"),dt.day());

			dsSkinWindow(m_vecButton[num]->m_hWnd,SKIN_TYPE_RADIOBUTTON,_T("DateOutMonth"),FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico1"), str1, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico2"), str2, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico3"), str3, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico4"), str4, FALSE);

			//pbtn->SetWindowText(StringUtl::TranslateString(StringUtl::IntToString(dt.day())));
			m_vecButton[num]->EnableWindow(TRUE);
			if (m_bMin)
			{
				if (premonthday<days_min)
				{
					m_vecButton[num]->EnableWindow(FALSE);
				}
			}
			if(m_bMax)
			{
				if(premonthday>days_max)
				{
					m_vecButton[num]->EnableWindow(FALSE);
				}
			}

		}
		//显示本页日历当前月
		for (long currmonthday = days_begin_month; currmonthday<= days_end_month; currmonthday++,num++)
		{
			_date_t dt;
			dt.from_days(currmonthday);
			//CButton* pbtn = _CreateButton(num);

			CString str1;
			str1.Format(_T("DC_%d_nor"),dt.day());
			CString str2;
			str2.Format(_T("DC_%d_over"),dt.day());
			CString str3;
			str3.Format(_T("DC_%d_down"),dt.day());
			CString str4;
			str4.Format(_T("DC_%d_check"),dt.day());

			dsSkinWindow(m_vecButton[num]->m_hWnd,SKIN_TYPE_RADIOBUTTON,_T("DateInMonth"),FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico1"), str1, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico2"), str2, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico3"), str3, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico4"), str4, FALSE);
			//pbtn->SetWindowText(StringUtl::TranslateString(StringUtl::IntToString(dt.day())));

			m_vecButton[num]->EnableWindow(TRUE);
			if (m_bMin)
			{
				if (currmonthday<days_min)
				{
					m_vecButton[num]->EnableWindow(FALSE);
				}
			}
			if(m_bMax)
			{
				if(currmonthday>days_max)
				{
					m_vecButton[num]->EnableWindow(FALSE);
				}
			}
		}
		//显示本页日历下个月的那几天
		for (long nextmonthday = days_end_month+1; nextmonthday<= days_end; nextmonthday++,num++)
		{
			_date_t dt;
			dt.from_days(nextmonthday);
			//CButton* pbtn = _CreateButton(num);

			CString str1;
			str1.Format(_T("DB_%d_nor"),dt.day());
			CString str2;
			str2.Format(_T("DB_%d_over"),dt.day());
			CString str3;
			str3.Format(_T("DB_%d_down"),dt.day());
			CString str4;
			str4.Format(_T("DB_%d_check"),dt.day());

			dsSkinWindow(m_vecButton[num]->m_hWnd,SKIN_TYPE_RADIOBUTTON,_T("DateOutMonth"),FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico1"), str1, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico2"), str2, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico3"), str3, FALSE);
			dsSetDrawItemValue( m_vecButton[num]->m_hWnd, _T("ico4"), str4, FALSE);
			//pbtn->SetWindowText(StringUtl::TranslateString(StringUtl::IntToString(dt.day())));

			m_vecButton[num]->EnableWindow(TRUE);
			if (m_bMin)
			{
				if (nextmonthday<days_min)
				{
					m_vecButton[num]->EnableWindow(FALSE);
				}
			}
			if(m_bMax)
			{
				if(nextmonthday>days_max)
				{
					m_vecButton[num]->EnableWindow(FALSE);
				}
			}
		}

		//选中当前日期
		if (days_curr>=days_begin && days_curr<=days_end)
		{

			CString str1;
			str1.Format(_T("DA_%d_nor"),curr_dtt.day());
			CString str2;
			str2.Format(_T("DA_%d_over"),curr_dtt.day());
			CString str3;
			str3.Format(_T("DA_%d_down"),curr_dtt.day());
			CString str4;
			str4.Format(_T("DA_%d_nor"),curr_dtt.day());

			CButton btn = GetDlgItem(CAL_BUTTONID+days_curr-days_begin);
			dsSkinWindow( btn.m_hWnd,SKIN_TYPE_RADIOBUTTON,_T("DateToday"),FALSE);
			dsSetDrawItemValue( btn.m_hWnd, _T("ico1"), str1, FALSE);
			dsSetDrawItemValue( btn.m_hWnd, _T("ico2"), str2, FALSE);
			dsSetDrawItemValue( btn.m_hWnd, _T("ico3"), str3, FALSE);
			dsSetDrawItemValue( btn.m_hWnd, _T("ico4"), str4, FALSE);

			btn.EnableWindow(TRUE);
			if (m_bMin)
			{
				if (days_curr<days_min)
				{
					btn.EnableWindow(FALSE);
				}
			}
			if(m_bMax)
			{
				if(days_curr>days_max)
				{
					btn.EnableWindow(FALSE);
				}
			}

			//CheckRadioButton(CAL_BUTTONID,CAL_BUTTONID+num,CAL_BUTTONID+days_curr-days_begin);
		}

		//选中当前显示日期
		if (days_show>=days_begin && days_show<=days_end)
		{
			CheckRadioButton(CAL_BUTTONID,CAL_BUTTONID+num,CAL_BUTTONID+days_show-days_begin);
			m_nChecked = days_show - days_begin;
		}

		//显示年月
		//_SetCtrlValue(IDC_BLMCAL_MONTH,show_dtt.month());
		//_SetCtrlValue(IDC_BLMCAL_YEAR,show_dtt.year());
		
		if (BlmKernel::Instance()->MainThreadLang() == 1028 
			|| BlmKernel::Instance()->MainThreadLang() == 2052)
		{
			LPTSTR lpszTmp = StringUtl::TranslateString(StringUtl::IntToString(show_dtt.year()));
			CString strMY = CString(lpszTmp)+_T("年")+MONTHSTR_CN[show_dtt.month()-1];
			delete lpszTmp;

			m_vecYearMonthStatic[0]->SetWindowText(strMY);
		}
		else
		{
			LPTSTR lpszTmp = StringUtl::TranslateString(StringUtl::IntToString(show_dtt.year()));
			CString strMY = MONTHSTR_EN[show_dtt.month()-1]+_T("  ")+ CString(lpszTmp);
			delete lpszTmp;

			m_vecYearMonthStatic[0]->SetWindowText(strMY);
		}
		
		

		//lpszTmp = StringUtl::TranslateString(StringUtl::IntToString(show_dtt.year()));
		//m_vecYearMonthStatic[1]->SetWindowText(lpszTmp);
		//delete lpszTmp;
		//显示时分，如果的话
		if (!m_bDateOrTime)
		{
			_SetMinHourVal(IDC_BLMCAL_HOUR_STATIC,show_dtt.hour());
			_SetMinHourVal(IDC_BLMCAL_MIN_STATIC,show_dtt._min());

			TCHAR tdesOK[MAX_PATH] = {'\0'},
				tdesCancel[MAX_PATH] = {'\0'};
			Boloomo::Core::BlmLoadString(_T("BLM_Shipping"), _T("MainDialog"), _T("IDOK"), tdesOK,_T("OK"));
			Boloomo::Core::BlmLoadString(_T("BLM_Shipping"), _T("MainDialog"), _T("IDCANCEL"), tdesCancel,_T("Cancel"));
			m_vecOkCancelBtn[0]->SetWindowText(tdesCancel);
			m_vecOkCancelBtn[1]->SetWindowText(tdesOK);
		}


		for (int i=0;i<7;i++)
		{	
			if (BlmKernel::Instance()->MainThreadLang() == 1028 
				|| BlmKernel::Instance()->MainThreadLang() == 2052)
			{
				m_vecWeekStatic[i]->SetWindowText(WEEKSTR_CN[i]);
			}
			else
			{
				m_vecWeekStatic[i]->SetWindowText(WEEKSTR_EN[i]);
			}
		}
		RedrawWindow();
	}

	void _CreateView()
	{
		//创建选择年月四个按钮,以及年月静态框
		CRect clientrc;
		GetClientRect(&clientrc);

		CRect btnrc(clientrc);
		btnrc.left += 18;
		btnrc.right = btnrc.left + 15;
		btnrc.top += 20;
		btnrc.bottom = btnrc.top + 20;
		CButton* pbtn1 = new CButton;
		pbtn1->Create(m_hWnd,&btnrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_YEAR_LEFT);
		dsSkinWindow(pbtn1->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_year_prev"), FALSE);
		pbtn1->SetWindowText(_T("<<"));
		m_vecChooseymBtn.push_back(pbtn1);


		btnrc.left = btnrc.right + 5;
		btnrc.right = btnrc.left + 15;
		//btnrc.top += 10;
		//btnrc.bottom = btnrc.top + 20;
		CButton* pbtn2 = new CButton;
		pbtn2->Create(m_hWnd,&btnrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_MONTH_LEFT);
		dsSkinWindow(pbtn2->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_month_prev"), FALSE);
		pbtn2->SetWindowText(_T("<"));
		m_vecChooseymBtn.push_back(pbtn2);

		btnrc.left = btnrc.right + 5;
		btnrc.right = btnrc.left + 140;
		//btnrc.top += 10;
		//btnrc.bottom = btnrc.top + 20;
		CStatic* stmonth = new CStatic;
		stmonth->Create(m_hWnd,&btnrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_MONTH);
		dsSkinWindow(stmonth->m_hWnd, SKIN_TYPE_STATIC, _T("blm_date_13"), FALSE);
		stmonth->SetWindowText(_T("June"));
		m_vecYearMonthStatic.push_back(stmonth);

		// 	btnrc.left = btnrc.right + 5;
		// 	btnrc.right = btnrc.left + 40;
		// 	//btnrc.top += 10;
		// 	//btnrc.bottom = btnrc.top + 20;
		// 	CStatic* styear = new CStatic;
		// 	styear->Create(m_hWnd,&btnrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_YEAR);
		// 	dsSkinWindow(styear->m_hWnd, SKIN_TYPE_STATIC, _T("blm_date_13"), FALSE);
		// 	styear->SetWindowText(_T("2011"));
		// 	m_vecYearMonthStatic.push_back(styear);


		btnrc.left = btnrc.right + 5;
		btnrc.right = btnrc.left + 15;
		//btnrc.top += 10;
		//btnrc.bottom = btnrc.top + 20;
		CButton* pbtn3 = new CButton;
		pbtn3->Create(m_hWnd,&btnrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_MONTH_RIGHT);
		dsSkinWindow(pbtn3->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_month_next"), FALSE);
		pbtn3->SetWindowText(_T(">"));
		m_vecChooseymBtn.push_back(pbtn3);


		btnrc.left = btnrc.right + 5;
		btnrc.right = btnrc.left + 15;
		//btnrc.top += 10;
		//btnrc.bottom = btnrc.top + 20;
		CButton* pbtn4 = new CButton;
		pbtn4->Create(m_hWnd,&btnrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_YEAR_RIGHT);
		dsSkinWindow(pbtn4->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_year_next"), FALSE);
		pbtn4->SetWindowText(_T(">>"));
		m_vecChooseymBtn.push_back(pbtn4);

		//创建星期几静态框
		CRect weekrc(clientrc);
		weekrc.top += 47;
		weekrc.bottom = weekrc.top + 18;
		weekrc.left += 13;
		for (int i=0;i<7;i++)
		{	
			weekrc.right = weekrc.left + 25;

			CStatic* pbtn = new CStatic;
			pbtn->Create(m_hWnd,&weekrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_WEEK1+i);
			dsSkinWindow(pbtn->m_hWnd, SKIN_TYPE_STATIC, _T("blm_date_10"), FALSE);
			m_vecWeekStatic.push_back(pbtn);

			//LPTSTR lpszTmp = StringUtl::TranslateString(WEEKSTR[i]);
			if (BlmKernel::Instance()->MainThreadLang() == 1028 
				|| BlmKernel::Instance()->MainThreadLang() == 2052)
			{
				pbtn->SetWindowText(WEEKSTR_CN[i]);
			}
			else
			{
				pbtn->SetWindowText(WEEKSTR_EN[i]);
			}
			
			//delete lpszTmp;

			weekrc.left = weekrc.right + 7;
		}

		if (!m_bDateOrTime)
		{
			//创建小时，分钟
			CRect hourrc(clientrc);
			hourrc.left += 80;
			hourrc.right = hourrc.left + 20;
			hourrc.bottom -= 75;
			hourrc.top = hourrc.bottom - 15;
			CButton* pbtn5 = new CButton;
			pbtn5->Create(m_hWnd,&hourrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_HOUR_UP);
			pbtn5->SetWindowText(_T("up"));
			dsSkinWindow(pbtn5->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_time_prev"), FALSE);
			pbtn5->ShowWindow(SW_HIDE);
			m_vecChoosehourBtn.push_back(pbtn5);

			CRect minrc(hourrc);
			minrc.left += 60;
			minrc.right = minrc.left + 20;
			CButton* pbtn7 = new CButton;
			pbtn7->Create(m_hWnd,&minrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_MIN_UP);
			pbtn7->SetWindowText(_T("up"));
			dsSkinWindow(pbtn7->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_time_prev"), FALSE);
			pbtn7->ShowWindow(SW_HIDE);
			m_vecChooseminBtn.push_back(pbtn7);


			hourrc.bottom += 30;
			hourrc.top = hourrc.bottom - 15;
			CButton* pbtn6 = new CButton;
			pbtn6->Create(m_hWnd,&hourrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_HOUR_DOWN);
			pbtn6->SetWindowText(_T("down"));
			dsSkinWindow(pbtn6->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_time_next"), FALSE);
			pbtn6->ShowWindow(SW_HIDE);
			m_vecChoosehourBtn.push_back(pbtn6);

			minrc.bottom += 30;
			minrc.top = minrc.bottom - 15;
			CButton* pbtn8 = new CButton;
			pbtn8->Create(m_hWnd,&minrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_MIN_DOWN);
			pbtn8->SetWindowText(_T("down"));
			dsSkinWindow(pbtn8->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_time_next"), FALSE);
			pbtn8->ShowWindow(SW_HIDE);
			m_vecChooseminBtn.push_back(pbtn8);


			hourrc.bottom -= 17;
			hourrc.top = hourrc.bottom - 15;
			CStatic* sthour = new CStatic;
			sthour->Create(m_hWnd,&hourrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_HOUR_STATIC);
			sthour->ModifyStyle(0, SS_NOTIFY);
			dsSkinWindow(sthour->m_hWnd, SKIN_TYPE_STATIC, _T("blm_date_12"), FALSE);
			sthour->SetWindowText(_T("09"));
			m_vecTimeStatic.push_back(sthour);

			hourrc.top -=2;
			hourrc.bottom +=2;
			hourrc.left -=5;
			hourrc.right +=5;
			CEdit* edithour = new CEdit;
			edithour->Create(m_hWnd,&hourrc,0,WS_CHILD|WS_TABSTOP,0,IDC_BLMCAL_HOUR_EDIT);
			dsSkinWindow( edithour->m_hWnd,SKIN_TYPE_EDIT,_T("blm_pagenum_edit"),TRUE);
			edithour->SetWindowText(_T(""));
			edithour->ModifyStyle(0,ES_NUMBER);
			edithour->ShowWindow(SW_HIDE);
			m_vecTimeEdit.push_back(edithour);


			minrc.bottom -= 17;
			minrc.top = minrc.bottom - 15;
			CStatic* stmin = new CStatic;
			stmin->Create(m_hWnd,&minrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP|SS_NOTIFY,0,IDC_BLMCAL_MIN_STATIC);
			dsSkinWindow(stmin->m_hWnd, SKIN_TYPE_STATIC, _T("blm_date_12"), FALSE);
			stmin->SetWindowText(_T("35"));
			m_vecTimeStatic.push_back(stmin);

			minrc.top -=2;
			minrc.bottom +=2;
			minrc.left -=5;
			minrc.right +=5;
			CEdit* editmin = new CEdit;
			editmin->Create(m_hWnd,&minrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_MIN_EDIT);
			dsSkinWindow( editmin->m_hWnd,SKIN_TYPE_EDIT,_T("Date_time_edit"),TRUE);
			editmin->SetWindowText(_T("35"));
			editmin->ModifyStyle(0,ES_NUMBER);
			editmin->ShowWindow(SW_HIDE);
			m_vecTimeEdit.push_back(editmin);


			//创建OK,CANCEL按钮
			CRect cancelrc(clientrc);
			cancelrc.bottom -= 12;
			cancelrc.top = cancelrc.bottom - 20;
			cancelrc.left += 20;
			cancelrc.right = cancelrc.left + 68;
			CButton* pbtncancel = new CButton;
			pbtncancel->Create(m_hWnd,&cancelrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_CANCEL);
			dsSkinWindow(pbtncancel->m_hWnd, SKIN_TYPE_BUTTON, _T("blm.btn.default"), FALSE);
			pbtncancel->SetWindowText(_T("Cancel"));
			m_vecOkCancelBtn.push_back(pbtncancel);

			CRect okrc(clientrc);
			okrc.bottom -= 12;
			okrc.top = okrc.bottom - 20;
			okrc.right -= 20;
			okrc.left = okrc.right - 68;
			CButton* pbtnok = new CButton;
			pbtnok->Create(m_hWnd,&okrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_OK);
			dsSkinWindow(pbtnok->m_hWnd, SKIN_TYPE_BUTTON, _T("blm.btn.default"), FALSE);
			pbtnok->SetWindowText(_T("OK"));
			m_vecOkCancelBtn.push_back(pbtnok);
		}


		//创建天按钮，固定6排，每排7个
		GetClientRect(&m_btnrc);
		m_btnrc.top +=66;
		m_btnrc.bottom -=20;
		m_btnrc.left +=13;
		m_btnrc.right -= 10;

		for(int i=0;i<CAL_BUTTONNUM;i++)
		{
			_CreateButton(i);
		}

	}

	CButton* _CreateButton(int num)
	{
		CRect btnrc(m_btnrc);
		btnrc.left = btnrc.left + (num%7)*(BTN_WIDTH);
		btnrc.right = btnrc.left + BTN_WIDTH;
		btnrc.top = btnrc.top + (num/7)*(BTN_HEIGTH);
		btnrc.bottom = btnrc.top + BTN_HEIGTH;
		CButton* pbtn = new CButton;
		pbtn->Create(m_hWnd,&btnrc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTORADIOBUTTON,0,CAL_BUTTONID+num);

		m_vecButton.push_back(pbtn);

		return pbtn;

	}

	void _SetMinHourVal(int nRes,int nVal)
	{
		char ctmp[3] = { '\0' };
		sprintf_s(ctmp,"%02d",nVal);
		string strtmp = ctmp;

		LPTSTR lpszTmp = StringUtl::TranslateString(strtmp);
		GetDlgItem(nRes).SetWindowText(lpszTmp);
		delete lpszTmp;
	}

	int _GetMinHourVal(int nRes)
	{
		LPTSTR lpszTmp = NULL;
		GetDlgItem(nRes).GetWindowText(lpszTmp);
		string strtmp = StringUtl::TranslateString(lpszTmp);

		return atoi(strtmp.c_str());
	}

private:

	_CSTRING_NS::CString m_strAppPath;
	_datetime_t m_showdt; //从外面传过来的显示的日期，默认值是当前日期
	_datetime_t	m_showingdt;//界面上显示的日期

	bool m_bDateOrTime; //true，则只显示日期;false，则显示日期与时间
	long m_ndaysBegin,m_ndaysEnd,m_nChecked;

	HWND m_parentMsgWnd;

	//////////////////////////////////////////////////////////////////////////
	//资源相关
	CRect m_btnrc; 	//日期button;
	vector<CButton*> m_vecButton;
	
	CRect m_chooseymbtnrc; //左右选择年,月btutton
	vector<CButton*> m_vecChooseymBtn;

	CRect m_ymrc;    //年月
	vector<CStatic*> m_vecYearMonthStatic;

	CRect m_weekrc;  //星期几静态框
	vector<CStatic*> m_vecWeekStatic;


	CRect m_choosehourbtnrc; //上下选择小时btutton
	vector<CButton*> m_vecChoosehourBtn;

	CRect m_chooseminbtnrc; //上下选择分btutton
	vector<CButton*> m_vecChooseminBtn;

	CRect m_timerc;  //时分静态框
	vector<CStatic*> m_vecTimeStatic;

	CRect m_timeeditrc;  //时分编辑框
	vector<CEdit*> m_vecTimeEdit;

	CRect m_okcancelrc;  //确定取消按钮
	vector<CButton*> m_vecOkCancelBtn;


	bool m_bMin,m_bMax;
	_datetime_t m_dtMin,m_dtMax;

};