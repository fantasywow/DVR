// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "dhplay.h"
#include "MainDlg.h"
#include "PreviewDlg.h"
#include "RecordManager.h"

int StreamDirectReadCallback(ULONG channelNumber, void * DataBuf,DWORD Length, int FrameType, void *context)
{
	CMainDlg * mainDlg;

	mainDlg = (CMainDlg *)context;
	mainDlg->SaveStreamData(channelNumber, DataBuf, Length, FrameType);

	return 0;
}



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
	UIAddChildWindowContainer(m_hWnd);
	InitRecordXMLfile();
	initDH();
	initPreviewDlg();
	initBottomButton();
	initTimeLabel();
	initSlide();
	initValue();
	//默认选中channel 0
	FocusChannel(0);
	//默认布局
	SetPreviewDlgLayout(PREVIEWLAYOUT4,0);
	//计时器
	SetTimer(1,1000);
	SetWindowPos(NULL,0,0,1000,700,SWP_SHOWWINDOW);
	CenterWindow();

	RegisterStreamDirectReadCallback(StreamDirectReadCallback, this);

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
	SetPreviewDlgLayout(PREVIEWLAYOUT1,0);
	return 0; 
}

LRESULT CMainDlg::OnBnClickedLayoutbutton4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetPreviewDlgLayout(PREVIEWLAYOUT4,0);
	return 0;
}

LRESULT CMainDlg::OnBnClickedLayoutbutton8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
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
	updateSetting();
	return 0;
}


LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SYSTEMTIME st;
	CString time;
	GetLocalTime(&st);
	time.Format(L"%d.%d.%d   %d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	GetDlgItem(IDC_TIME_LABEL).SetWindowText(time);

	if (st.wMinute==0&&st.wSecond==0)
	{
		CheckRecordPlan(st.wDayOfWeek,st.wHour);
	}

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

void CMainDlg::updateSetting()
{
	//todo
}

void CMainDlg::initSlide()
{
	m_hueTitle.Attach(GetDlgItem(IDC_STATIC_HUE));
	m_hueTitle.SetWindowText(L"色相");
	m_hueTitle.SetWindowPos(NULL,730,130,50,20,SWP_SHOWWINDOW);
	m_hueSlide.Attach(GetDlgItem(IDC_HUE_SLIDER));
	m_hueSlide.SetWindowPos(NULL,730,150,200,30,SWP_SHOWWINDOW);
	m_hueSlide.SetRange(0,255,TRUE);
	m_saturationTitle.Attach(GetDlgItem(IDC_STATIC_SATURATION));
	m_saturationTitle.SetWindowText(L"饱和度");
	m_saturationTitle.SetWindowPos(NULL,730,180,50,20,SWP_SHOWWINDOW);
	m_saturationSlide.Attach(GetDlgItem(IDC_SATURATION_SLIDER));
	m_saturationSlide.SetWindowPos(NULL,730,200,200,30,SWP_SHOWWINDOW);
	m_saturationSlide.SetRange(0,255,TRUE);
	m_brightnessTitle.Attach(GetDlgItem(IDC_STATIC_BRGHTNESS));
	m_brightnessTitle.SetWindowText(L"亮度");
	m_brightnessTitle.SetWindowPos(NULL,730,230,50,20,SWP_SHOWWINDOW);
	m_brightnessSlide.Attach(GetDlgItem(IDC_BRGHTNESS_SLIDER));
	m_brightnessSlide.SetWindowPos(NULL,730,250,200,30,SWP_SHOWWINDOW);
	m_brightnessSlide.SetRange(0,255,TRUE);
	m_contrastTitle.Attach(GetDlgItem(IDC_STATIC_CONTRAST));
	m_contrastTitle.SetWindowText(L"对比度");
	m_contrastTitle.SetWindowPos(NULL,730,280,50,20,SWP_SHOWWINDOW);
	m_contrastSlide.Attach(GetDlgItem(IDC_CONTRAST_SLIDER));
	m_contrastSlide.SetWindowPos(NULL,730,300,200,30,SWP_SHOWWINDOW);
	m_contrastSlide.SetRange(0,255,TRUE);
}

void CMainDlg::initTimeLabel()
{
	GetDlgItem(IDC_TIME_LABEL).SetWindowPos(NULL,750,50,200,50,SWP_SHOWWINDOW);
	CString time;
	GetLocalTime(&m_systemTime);
	time.Format(L"%d.%d.%d   %d:%02d:%02d",m_systemTime.wYear,m_systemTime.wMonth,m_systemTime.wDay,m_systemTime.wHour,m_systemTime.wMinute,m_systemTime.wSecond);
	GetDlgItem(IDC_TIME_LABEL).SetWindowText(time);
}

void CMainDlg::initBottomButton()
{
	GetDlgItem(IDC_BUTTON_CHOOSEFILE).SetWindowPos(NULL,50,590,100,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_LAYOUTBUTTON1).SetWindowPos(NULL,730,590,20,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_LAYOUTBUTTON4).SetWindowPos(NULL,755,590,20,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_LAYOUTBUTTON8).SetWindowPos(NULL,780,590,20,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_SETTINGBUTTON).SetWindowPos(NULL,805,590,20,20,SWP_SHOWWINDOW);
}

void CMainDlg::initDH()
{
	int iBoardNum;
	iBoardNum = InitDSPs();
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
			GetLocalTime(&m_systemTime);
			SetupDateTime(m_channelHandle[i], &m_systemTime);
			RECT rc = {0,30,342,248};
			StartVideoPreview(m_channelHandle[i],m_previewDlg[i],&rc,FALSE,0,25);
		}
	}
}

void CMainDlg::initPreviewDlg()
{
	for(int i=0;i<8;i++){
		m_previewDlg[i].Create(m_hWnd,i);
	}
}

void CMainDlg::initValue()
{
	m_isFullScreen = FALSE;
	m_settingDlg = new CSettingDlg(m_channelHandle);
}



LRESULT CMainDlg::OnBnClickedButtonChoosefile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
// 	TCHAR szPathName[MAX_PATH];  
// 	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };//or  {sizeof (OPENFILENAME)}  
// 	ofn.hwndOwner =GetForegroundWindow();// 打开OR保存文件对话框的父窗口  
// 	ofn.lpstrFilter = TEXT("264\0*.264\0");   
// 	lstrcpy(szPathName, TEXT(""));  
// 	ofn.lpstrFile = szPathName;  
// 	ofn.nMaxFile = sizeof(szPathName);//存放用户选择文件的 路径及文件名 缓冲区  
// 	ofn.lpstrTitle = TEXT("选择文件");//选择文件对话框标题  
// 	ofn.lpstrInitialDir=m_settingDlg->m_capturePath;//
// 	BOOL bOk = GetOpenFileName(&ofn);//调用对话框打开文件  
// 	if (bOk)  
// 	{  
// 		WideCharToMultiByte (CP_OEMCP,NULL,szPathName,-1,psText,MAX_PATH*2,NULL,FALSE);		
	m_playDlg = new CPlayDlg();
	m_playDlg->Create(NULL);
	m_playDlg->ShowWindow(TRUE);
// 	
// 	} 
	return 0;
}



void CMainDlg::SaveStreamData(int iChannel, void * pData, int iNum, int iFrameType)
{
	int iCH = iChannel;

	if (iChannel < 0 || iChannel >= BLM_CHANNEL_MAX)
	{
		return;
	}

	else if (iFrameType & PktSubSysHeader || iFrameType & PktSubIFrames || iFrameType & PktSubPFrames || iFrameType & PktSubAudioFrames) 
	{
		iCH += BLM_CHANNEL_MAX;
	}

	WriteFile(m_fileHandle[iCH], pData, iNum,NULL,NULL);

}


void CMainDlg::StartCaptureVideo(int iChannel,bool sub)
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	SetupDateTime(m_channelHandle[iChannel], &systemTime);

	CString fileName;
	fileName.Format(L"%s%dY%dM%dD%dH%d",m_settingDlg->m_capturePath,systemTime.wYear,systemTime.wMonth,systemTime.wDay,systemTime.wHour,iChannel);
	m_fileHandle[iChannel] = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	InsertOneRecord(systemTime,fileName);
	if (sub) 
	{
		fileName.Format(L"%s%dY%dM%dD%dH%dSub",m_settingDlg->m_capturePath,systemTime.wYear,systemTime.wMonth,systemTime.wDay,systemTime.wHour,iChannel);
		m_fileHandle[iChannel+BLM_CHANNEL_MAX] = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		InsertOneRecord(systemTime,fileName);

		SetupSubChannel(m_channelHandle[iChannel], 1);
		CaptureIFrame(m_channelHandle[iChannel]);
		StartSubVideoCapture(m_channelHandle[iChannel]);
	}

	SetupSubChannel(m_channelHandle[iChannel], 0);
	CaptureIFrame(m_channelHandle[iChannel]);
	int iRet = StartVideoCapture(m_channelHandle[iChannel]);

	if (iRet != SS_SUCCESS)
	{
		MessageBox(L"录制失败");
	}
	else
	{
		m_isVideoCapture[iChannel] = TRUE;
	}
}

void CMainDlg::StopCaptureVideo(int iChannel,bool sub)
{
	StopVideoCapture(m_channelHandle[iChannel]);
	if (sub) 
	{
		StopSubVideoCapture(m_channelHandle[iChannel]);
	}
	CloseHandle(m_fileHandle[iChannel]);
	if(sub)
	{
		CloseHandle(m_fileHandle[iChannel+BLM_CHANNEL_MAX]);
	}

	m_isVideoCapture[iChannel] = FALSE;

}

void CMainDlg::CheckRecordPlan( int dayofWeek,int hour )
{
	for (int i=0;i<BLM_CHANNEL_MAX;i++)
	{
		if (m_channelHandle[i]!=INVALID_HANDLE_VALUE)
		{
			if (m_settingDlg->m_recodePlan[i][dayofWeek][hour]==TRUE&&m_isVideoCapture[i]==FALSE)
			{
				StartCaptureVideo(i,m_settingDlg->m_encodeSetting[i].sub);
			}
			if (m_settingDlg->m_recodePlan[i][dayofWeek][hour]==FALSE&&m_isVideoCapture[i]==TRUE)
			{
				StopCaptureVideo(i,m_settingDlg->m_encodeSetting[i].sub);
			}
		}
	}
}
