// MainDlg.h : interface of the CBlmDateTimeCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#define IDC_BLMCAL_BUTTON  18080

#include "BlmCalDlg.h"

#include "Dtz.h"
#include "StringUtl.h"
#include "BlmKernel.h"

using namespace Boloomo::Util;


//#define UM_DATECHANGED WM_USER+1001
#define UM_DATETIMECHANGED WM_USER+1022

enum BLMDATETIMECTRL_STYLE
{
	STYLE_DEFAULT = 0,
	STYLE_VSLTRACE,
	STYLE_UNDERLINEWHITE,//下划线白色
	STYLE_UNDERLINEGRAY,//下划线灰绿色
	STYLE_ACCOUNT
};


class CBlmDateTimeCtrl : public CDialogImpl<CBlmDateTimeCtrl>, 
		public CUpdateUI<CBlmDateTimeCtrl>,
		public CMessageFilter, 
		public CIdleHandler
{
public:
	enum { IDD = IDD_BLMCALRESDLG };


	BEGIN_UPDATE_UI_MAP(CBlmDateTimeCtrl)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CBlmDateTimeCtrl)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(UM_CHANGEDATE, OnChangeDate)
		MESSAGE_HANDLER(UM_CHANGEDATETIME, OnChangeDateTime)
		COMMAND_HANDLER(IDC_BLMCAL_BUTTON,BN_CLICKED,OnShowCal)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle()
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

		//初始化
		//InitBlmCalendar(_datetime_t::now(),"%Y-%m-%d");
		if (m_strFormat.find('H')!=-1)
			m_dlgcal.SetShowDate(false,m_showdt);
		else
			m_dlgcal.SetShowDate(true,m_showdt);
		
		//皮肤化
		dsSkinWindow(m_hWnd, SKIN_TYPE_DIALOG, _T("transparent_childdialog"), FALSE);
		CRect rc;
		GetClientRect(&rc);
		pbtn = new CButton;
		pbtn->Create(m_hWnd,&rc,0,WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,IDC_BLMCAL_BUTTON);
		_setdtstyle();
		
		CRect wndrc;
		pbtn->GetWindowRect(&wndrc);
		MoveWindow(wndrc);

		//显示时间
		_setdtvalue();
		m_dlgcal.Create(m_hWnd);
		m_dlgcal.SetMsgParent(m_hWnd);
		m_dlgcal.ShowWindow(SW_HIDE);

		return 0;
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

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CRect rc;
		GetClientRect(&rc);
		pbtn->MoveWindow(&rc);
		return 0;
	}	

// 	LRESULT OnChangeDate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
// 	{
// // 		long days = (long)wParam;
// // 		_datetime_t dtt;
// // 		dtt.from_days(days);
// 		m_showdt = dtt;
// 		_setdtvalue();
// 
// 		SendMessage(GetParent(),UM_DATECHANGED,(WPARAM)&m_showdt,0);
// 		return 0;
// 	}
	LRESULT OnChangeDateTime(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		//long days = (long)wParam;
		//_datetime_t dtt;
		//dtt.from_days(days);

		//m_showdt = dtt;
		//_setdtvalue();
		m_showdt = *(_datetime_t*)wParam;

		_setdtvalue();
		SendMessage(GetParent(),UM_DATETIMECHANGED,(WPARAM)&m_showdt,(LPARAM)m_hWnd);

		return 0;
	}
	
	LRESULT OnShowCal(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (m_dlgcal.IsWindow() && m_dlgcal.IsWindowVisible())
		{
			m_dlgcal.ShowWindow(SW_HIDE);
			//m_dlgcal.DestroyWindow();
		}	
		else
		{
			if (!m_dlgcal.IsWindow())
			{
				m_dlgcal.Create(m_hWnd);
			}
			m_dlgcal.ResetShowDateView(m_showdt);

			CRect rc;
			pbtn->GetWindowRect(&rc);
			CRect moverc;
			m_dlgcal.GetWindowRect(&moverc);

			int nyscreen = ::GetSystemMetrics(SM_CYFULLSCREEN); 
			int nxscreen = ::GetSystemMetrics(SM_CXFULLSCREEN);
			if (rc.bottom + moverc.Height() >nyscreen)
			{
				CRect torc;

				if(rc.left + moverc.Width() < nxscreen)
				{
					torc.left = rc.left - 10;
					torc.right = torc.left + moverc.Width();
				}
				else
				{
					torc.right = rc.right;
					torc.left = rc.right - moverc.Width();
				}
			
				torc.bottom= rc.top - 5;
				torc.top = torc.bottom - moverc.Height();
				m_dlgcal.MoveWindow(&torc);
			}
			else
			{
				//默认向下显示日历
				CRect torc;

				if(rc.left + moverc.Width() < nxscreen)
				{
					torc.left = rc.left - 10;
					torc.right = torc.left + moverc.Width();
				}
				else
				{
					torc.right = rc.right;
					torc.left = rc.right - moverc.Width();
				}

				torc.top = rc.top + 25;
				torc.bottom = torc.top + moverc.Height();
				m_dlgcal.MoveWindow(&torc);
			}
			m_dlgcal.ShowWindow(SW_SHOW);
			
		}	
		return 0;
	}

public:
	CBlmDateTimeCtrl():m_strFormat("%Y-%m-%d %H:%M:%S"),
		m_showdt(_datetime_t::now()),
		m_nStyle(STYLE_DEFAULT),
		pbtn(NULL)
	{
	}
	~CBlmDateTimeCtrl()
	{
		if(pbtn)
		{
			if(pbtn->IsWindow())
				pbtn->DestroyWindow();
			delete pbtn;
		}

		if (m_dlgcal.IsWindow())
		{
			m_dlgcal.DestroyWindow();
		}
	}
	//设置显示的样式
	void SetBlmCalendarStyle(BLMDATETIMECTRL_STYLE nstyle)
	{
		if (m_nStyle != nstyle)
		{
			m_nStyle = nstyle;

			if (IsWindow())
			{
				_setdtstyle();
			}
		}
	}

	//只设置格式
	void SetBlmCalendarFormat(const string& szformat = "%Y-%m-%d %H:%M:%S")
	{
		m_strFormat = szformat;
		if (IsWindow())
			_setdtvalue();
	}
	//只设置日期
	void SetBlmCalendarDateTime(const _datetime_t& showdt = _datetime_t::now())
	{
		m_showdt = showdt;
		if (IsWindow())
			_setdtvalue();
	}
	//设置格式和初始日期
	void InitBlmCalendar(const _datetime_t& showdt = _datetime_t::now(),const string& szformat = "%Y-%m-%d %H:%M:%S",BLMDATETIMECTRL_STYLE nstyle = STYLE_DEFAULT)
	{
		m_showdt = showdt;
		m_strFormat = szformat;
		m_nStyle = nstyle;
		if (IsWindow())
		{
			_setdtvalue();
			_setdtstyle();
		}
	}
	//获得当前日期
	_datetime_t GetCurrDateTime(){return m_showdt;}
	//隐藏对话框
	void HideDateDlg()
	{
		m_dlgcal.ShowWindow(SW_HIDE);
	}
	//设置有效期

	void SetBlmCalendarRange(bool bsetmin = true,
		const _datetime_t& mindt = _datetime_t::now(),
		bool bsetmax = true,
		const _datetime_t& maxdt = _datetime_t::now())
	{
		m_dlgcal.SetRange(bsetmin,mindt,bsetmax,maxdt);
	}

	void BlmEnableWindow(BOOL benable)
	{
		if (IsWindow())
		{
			GetDlgItem(IDC_BLMCAL_BUTTON).EnableWindow(benable);
		}
	}



protected:
private:
	_datetime_t m_showdt; //从外面传过来的显示的日期，默认值是当前日期
	string m_strFormat;
	BLMDATETIMECTRL_STYLE m_nStyle;

	CButton* pbtn;
	CBlmCalDlg m_dlgcal;

	void _setdtvalue()
	{
		string strdate = DateTimeUtl::Format(m_showdt,m_strFormat);
		LPTSTR lpszStr = StringUtl::TranslateString(strdate);
		pbtn->SetWindowText(lpszStr);
		delete lpszStr;
	}
	void _setdtstyle()
	{	
		switch (m_nStyle)
		{
			case STYLE_DEFAULT:
				dsSkinWindow(pbtn->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_Select_full"), FALSE);
				break;
			case STYLE_VSLTRACE:
				dsSkinWindow(pbtn->m_hWnd, SKIN_TYPE_BUTTON, _T("TimeSelTrace"), FALSE);
				break;
			case STYLE_UNDERLINEWHITE:
				dsSkinWindow(pbtn->m_hWnd, SKIN_TYPE_BUTTON, _T("PilotDateSel"), FALSE);
				break;
			case STYLE_UNDERLINEGRAY:
				dsSkinWindow(pbtn->m_hWnd, SKIN_TYPE_BUTTON, _T("TimeSelTrace"), FALSE);
				break;
			case STYLE_ACCOUNT:
				dsSkinWindow(pbtn->m_hWnd, SKIN_TYPE_BUTTON, _T("AccountDateBtn"), FALSE);
				break;	
			default:
				dsSkinWindow(pbtn->m_hWnd, SKIN_TYPE_BUTTON, _T("Date_Select_full"), FALSE);
				break;
		}
	}

};