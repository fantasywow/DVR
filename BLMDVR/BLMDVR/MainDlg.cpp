// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "PreviewDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	//UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	SetWindowPos(NULL,0,0,1000,700,SWP_HIDEWINDOW);
	CenterWindow();

	m_isFullScreen = FALSE;

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	SYSTEMTIME systemTime;

	int iBoardNum;
	iBoardNum = InitDSPs();
	for(int i=0;i<8;i++){
		m_previewDlg[i].Create(m_hWnd,i);
	}
  	if (iBoardNum <= 0)
  	{
  		MessageBox(L"大华卡初始化失败!\n");
		for (int i=0;i<8;i++)
		{
			m_channelHandle[i] = INVALID_HANDLE_VALUE;
		}
  	}else{
 		for (int i=0;i<8;i++)
 		{	
			m_channelHandle[i] = ChannelOpen(i);
			GetLocalTime(&systemTime);
			SetupDateTime(m_channelHandle[i], &systemTime);
			RECT rc = {0,30,342,248};
			StartVideoPreview(m_channelHandle[i],m_previewDlg[i],&rc,FALSE,0,25);
		}
	}
	//底部按钮
	GetDlgItem(IDC_LAYOUTBUTTON1).SetWindowPos(NULL,730,590,20,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_LAYOUTBUTTON4).SetWindowPos(NULL,755,590,20,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_LAYOUTBUTTON8).SetWindowPos(NULL,780,590,20,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_SETTINGBUTTON).SetWindowPos(NULL,805,590,20,20,SWP_SHOWWINDOW);
	//时间
	GetDlgItem(IDC_TIME_LABEL).SetWindowPos(NULL,730,100,200,50,SWP_SHOWWINDOW);
	CString time;
	GetLocalTime(&systemTime);
	time.Format(L"%d.%d.%d   %d:%02d:%02d",systemTime.wYear,systemTime.wMonth,systemTime.wDay,systemTime.wHour,systemTime.wMinute,systemTime.wSecond);
	GetDlgItem(IDC_TIME_LABEL).SetWindowText(time);
	//滑动条
	m_hueSlide.Attach(GetDlgItem(IDC_HUE_SLIDER));
	m_hueSlide.SetWindowPos(NULL,730,150,200,50,SWP_SHOWWINDOW);
	m_hueSlide.SetRange(0,255,TRUE);
	m_saturationSlide.Attach(GetDlgItem(IDC_SATURATION_SLIDER));
	m_saturationSlide.SetWindowPos(NULL,730,200,200,50,SWP_SHOWWINDOW);
	m_saturationSlide.SetRange(0,255,TRUE);
	m_brightnessSlide.Attach(GetDlgItem(IDC_BRGHTNESS_SLIDER));
	m_brightnessSlide.SetWindowPos(NULL,730,250,200,50,SWP_SHOWWINDOW);
	m_brightnessSlide.SetRange(0,255,TRUE);
	m_contrastSlide.Attach(GetDlgItem(IDC_CONTRAST_SLIDER));
	m_contrastSlide.SetWindowPos(NULL,730,300,200,50,SWP_SHOWWINDOW);
	m_contrastSlide.SetRange(0,255,TRUE);
	//默认选中channel 0
	FocusChannel(0);
	//默认布局
	SetPreviewDlgLayout(PREVIEWLAYOUT4,0);
	//计时器
	SetTimer(1,1000);

	ShowWindow(SW_SHOW);
	m_settingDlg = new CSettingDlg(m_channelHandle);
	UIAddChildWindowContainer(m_hWnd);
	return TRUE;
}

LRESULT CMainDlg::OnCapturePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	unsigned long dwSize = 1920*1080*2;
	UCHAR* pJpegBuf = new UCHAR[dwSize];
	if(pJpegBuf == NULL)
	{
		return FALSE;
	}
	int iRet = GetJpegImage(m_channelHandle[(int)lParam], pJpegBuf, &dwSize, 100);
	if(iRet != 0)
	{
		delete pJpegBuf;
		MessageBox(L"fail to capture a Jpeg image!");
		return FALSE;
	}
	HANDLE hJpgFile = CreateFile(L"test.jpeg", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hJpgFile == INVALID_HANDLE_VALUE)
	{
		delete pJpegBuf;
		MessageBox(L"fail to capture a Jpeg image!");
		return FALSE;          
	}
	if(!WriteFile(hJpgFile, pJpegBuf, dwSize, NULL, NULL))
	{
		delete pJpegBuf;
		CloseHandle(hJpgFile);
		MessageBox(L"fail to capture a Jpeg image!");
		return FALSE;          
	}
	delete pJpegBuf;
	CloseHandle(hJpgFile);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}



void CMainDlg::SetPreviewDlgLayout(PreviewLayout layout,int channelID)
{
	if (layout == PREVIEWLAYOUT1)
	{	
		for(int i =0 ;i<8;i++){
			m_previewDlg[i].ShowWindow(SW_HIDE);
		}
		m_previewDlg[0].SetWindowPos(NULL,0,0,704,576,SWP_SHOWWINDOW);
		m_lastLayout = layout;
	}
	if (layout == PREVIEWLAYOUT4)
	{
		m_previewDlg[0].SetWindowPos(NULL,0,0,352,288,SWP_SHOWWINDOW);
		m_previewDlg[1].SetWindowPos(NULL,352,0,352,288,SWP_SHOWWINDOW);
		m_previewDlg[2].SetWindowPos(NULL,0,288,352,288,SWP_SHOWWINDOW);
		m_previewDlg[3].SetWindowPos(NULL,352,288,352,288,SWP_SHOWWINDOW);
		for (int i=4;i<8;i++){
			m_previewDlg[i].ShowWindow(SW_HIDE);
		}
		m_lastLayout = layout;
	}
	if (layout == PREVIEWLAYOUT8)
	{
		m_previewDlg[0].SetWindowPos(NULL,0,0,528,432,SWP_SHOWWINDOW);
		m_previewDlg[1].SetWindowPos(NULL,528,0,176,144,SWP_SHOWWINDOW);
		m_previewDlg[2].SetWindowPos(NULL,528,144,176,144,SWP_SHOWWINDOW);
		m_previewDlg[3].SetWindowPos(NULL,528,288,176,144,SWP_SHOWWINDOW);
		m_previewDlg[4].SetWindowPos(NULL,0,432,176,144,SWP_SHOWWINDOW);
		m_previewDlg[5].SetWindowPos(NULL,176,432,176,144,SWP_SHOWWINDOW);
		m_previewDlg[6].SetWindowPos(NULL,352,432,176,144,SWP_SHOWWINDOW);
		m_previewDlg[7].SetWindowPos(NULL,528,432,176,144,SWP_SHOWWINDOW);
		m_lastLayout = layout;
	}	
	if (layout == PREVIEWLAYOUTFULL)
	{
		for(int i =0 ;i<8;i++){
			m_previewDlg[i].ShowWindow(SW_HIDE);
		}
		m_previewDlg[channelID].SetWindowPos(NULL,0,0,704,576,SWP_SHOWWINDOW);
	}

}

LRESULT CMainDlg::OnBnClickedLayoutbutton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	SetPreviewDlgLayout(PREVIEWLAYOUT1,0);
	return 0; 
}

LRESULT CMainDlg::OnBnClickedLayoutbutton4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	SetPreviewDlgLayout(PREVIEWLAYOUT4,0);
	return 0;
}

LRESULT CMainDlg::OnBnClickedLayoutbutton8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	SetPreviewDlgLayout(PREVIEWLAYOUT8,0);
	return 0;
}

LRESULT CMainDlg::OnPreviewDBLCLK(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (m_isFullScreen)
	{
		SetPreviewDlgLayout(m_lastLayout,0);
		m_isFullScreen = FALSE;
	}else{
		SetPreviewDlgLayout(PREVIEWLAYOUTFULL,(int)lParam);
		m_isFullScreen = TRUE;
	}
	return 0;
}


LRESULT CMainDlg::OnBnClickedSettingbutton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_settingDlg->DoModal();
	return 0;
}


LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SYSTEMTIME st;
	CString time;
	GetLocalTime(&st);
	time.Format(L"%d.%d.%d   %d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	GetDlgItem(IDC_TIME_LABEL).SetWindowText(time);
	return 0;
}

LRESULT CMainDlg::OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	int iRet;
	if (m_channelHandle[m_focusChannel]!=INVALID_HANDLE_VALUE)
	{
		iRet = SetVideoPara(m_channelHandle[0],m_brightnessSlide.GetPos(),m_contrastSlide.GetPos(),m_saturationSlide.GetPos(),m_hueSlide.GetPos());
	}
	return true;
}

LRESULT CMainDlg::OnPreviewFocus( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
	if (m_focusChannel!=(int)lParam)
	{
		m_previewDlg[m_focusChannel].SendMessage(BM_PREVIEW_UNFOCUS,0,0);
		FocusChannel((int)lParam);
		CString test;
		test.Format(L"focus:%d",m_focusChannel);
		ATLTRACE(test);
	}
	
	return true;
}

void CMainDlg::FocusChannel( int channelID )
{
	m_focusChannel = channelID;
	if (m_channelHandle[m_focusChannel]==INVALID_HANDLE_VALUE)
	{
		GetDlgItem(IDC_HUE_SLIDER).EnableWindow(FALSE);
		GetDlgItem(IDC_SATURATION_SLIDER).EnableWindow(FALSE);
		GetDlgItem(IDC_BRGHTNESS_SLIDER).EnableWindow(FALSE);
		GetDlgItem(IDC_CONTRAST_SLIDER).EnableWindow(FALSE);
	}else{
		int hue,saturation,brightness,contrast;
		GetVideoPara(m_channelHandle[m_focusChannel],0,&brightness,&contrast,&saturation,&hue);

		GetDlgItem(IDC_HUE_SLIDER).EnableWindow(TRUE);
		GetDlgItem(IDC_SATURATION_SLIDER).EnableWindow(TRUE);
		GetDlgItem(IDC_BRGHTNESS_SLIDER).EnableWindow(TRUE);
		GetDlgItem(IDC_CONTRAST_SLIDER).EnableWindow(TRUE);

		m_hueSlide.SetPos(hue);
		m_saturationSlide.SetPos(saturation);
		m_brightnessSlide.SetPos(brightness);
		m_contrastSlide.SetPos(contrast);
	}
}


