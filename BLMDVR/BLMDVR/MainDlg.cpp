// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "dhplay.h"
#include "dhvecsystem.h"
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

	InitRecordIndex();
	initDH();
	initPreviewDlg();
	initBottomButton();
	initTimeLabel();
	initSlide();
	initValue();
	initCradleButton();
	//默认选中channel 0
	FocusChannel(0);
	//默认布局
	SetPreviewDlgLayout(PREVIEWLAYOUT4,0);
	//计时器 时钟
	SetTimer(1,1000);
	//录像
	SetTimer(2,1000);
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
		MessageBox("fail to capture a Jpeg image!");
		return FALSE;
	}
	HANDLE hJpgFile = CreateFile("test.jpeg", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hJpgFile == INVALID_HANDLE_VALUE)
	{
		delete pJpegBuf;
		MessageBox("fail to capture a Jpeg image!");
		return FALSE;          
	}
	if(!WriteFile(hJpgFile, pJpegBuf, dwSize, NULL, NULL))
	{
		delete pJpegBuf;
		CloseHandle(hJpgFile);
		MessageBox("fail to capture a Jpeg image!");
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


LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	int timerID = (int)wParam;
	SYSTEMTIME st;
	CString time;
	GetLocalTime(&st);
	time.Format("%d.%d.%d   %d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	GetDlgItem(IDC_TIME_LABEL).SetWindowText(time);
	
	if (timerID ==2)
	{
		if (st.wMinute==0&&st.wSecond==0)
		{
			CheckRecordPlan(st.wDayOfWeek,st.wHour);
		}
	}

	return 0;
}

LRESULT CMainDlg::OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	int iRet;
	if (m_channelHandle[m_focusChannel]!=INVALID_HANDLE_VALUE)
	{
		iRet = SetVideoPara(m_channelHandle[m_focusChannel],m_brightnessSlide.GetPos(),m_contrastSlide.GetPos(),m_saturationSlide.GetPos(),m_hueSlide.GetPos());
	}
	return true;
}

LRESULT CMainDlg::OnPreviewFocus( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
	if (m_focusChannel!=(int)lParam)
	{
		m_previewDlg[m_focusChannel].SendMessage(BM_PREVIEW_UNFOCUS,0,0);
		FocusChannel((int)lParam);
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
		GetDlgItem(IDC_BUTTON_DEFAULT).EnableWindow(FALSE);
	}else{
		int hue,saturation,brightness,contrast;
		GetVideoPara(m_channelHandle[m_focusChannel],0,&brightness,&contrast,&saturation,&hue);

		GetDlgItem(IDC_HUE_SLIDER).EnableWindow(TRUE);
		GetDlgItem(IDC_SATURATION_SLIDER).EnableWindow(TRUE);
		GetDlgItem(IDC_BRGHTNESS_SLIDER).EnableWindow(TRUE);
		GetDlgItem(IDC_CONTRAST_SLIDER).EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DEFAULT).EnableWindow(TRUE);

		m_hueSlide.SetPos(hue);
		m_saturationSlide.SetPos(saturation);
		m_brightnessSlide.SetPos(brightness);
		m_contrastSlide.SetPos(contrast);
	}
}

void CMainDlg::updateSetting()
{
	for (int i = 0;i<BLM_CHANNEL_MAX;i++)
	{
		if (m_channelHandle[i] != INVALID_HANDLE_VALUE)
		{
			//取值范围：12～30
			//系统默认：16
			int quant[5]={12,16,20,24,30};
			SetDefaultQuant(m_channelHandle[i],quant[m_settingDlg->m_encodeSetting[i].quality],NULL,NULL);
			/*typedef enum 
			{
				ENC_QCIF_FORMAT = 1,
				ENC_CIF_FORMAT = 2,
				ENC_MCIF_FORMAT = 3,
				ENC_HCIF_FORMAT = 4,
				ENC_2CIF_FORMAT = 5,
				ENC_HD1_FORMAT = 6,
				ENC_4CIF_FORMAT = 7,
			}PictureFormat_t;
			//PictureForamt_t
			//m_format.AddString("CIF");
			//m_format.AddString("QCIF");
			//m_format.AddString("2CIF");
			//m_format.AddString("D1");
			*/
			int format[4] = {1,2,5,6};
			SetEncoderPictureFormat(m_channelHandle[i],(PictureFormat_t)format[m_settingDlg->m_encodeSetting[i].format]);
			//N 制：1,2,3,4,5,6,7,10,15,30；
			//P 制：1,2,3,4,5,6,8,12,25。
			int frameRateP[10] ={1,2,3,4,5,6,7,10,15,30};
			int frameRateN[9] ={1,2,3,4,5,6,8,12,25};
			VideoStandard_t vs;
			int a,b,c,d;//useless
			GetVideoPara(m_channelHandle[i],&vs,&a,&b,&c,&d);
			if (vs==STANDARD_PAL)
			{
				SetIBPMode(m_channelHandle[i],25,0,0,frameRateP[m_settingDlg->m_encodeSetting[i].frameRate]);
			}else{
				SetIBPMode(m_channelHandle[i],25,0,0,frameRateN[m_settingDlg->m_encodeSetting[i].frameRate]);
			}
			int maxBits[5];
			SetupBitrateControl(m_channelHandle[i],5*1024*1024);
			if (m_settingDlg->m_encodeSetting[i].sub)
			{
				SetSubEncoderPictureFormat(m_channelHandle[i],(PictureFormat_t)format[m_settingDlg->m_encodeSetting[i].format_sub]);
				SetupSubChannel(m_channelHandle[i],1);
				SetDefaultQuant(m_channelHandle[i],quant[m_settingDlg->m_encodeSetting[i].quality_sub],NULL,NULL);
				if (vs==STANDARD_PAL)
				{
					SetIBPMode(m_channelHandle[i],25,0,0,frameRateP[m_settingDlg->m_encodeSetting[i].frameRate_sub]);
				}else{
					SetIBPMode(m_channelHandle[i],25,0,0,frameRateN[m_settingDlg->m_encodeSetting[i].frameRate_sub]);
				}
				SetupBitrateControl(m_channelHandle[i],5*1024*1024);
				SetupSubChannel(m_channelHandle[i],0);
			}

		}
	}
}

void CMainDlg::initSlide()
{
	m_hueTitle.Attach(GetDlgItem(IDC_STATIC_HUE));
	m_hueTitle.SetWindowText("色相");
	m_hueTitle.SetWindowPos(NULL,730,100,50,20,SWP_SHOWWINDOW);
	m_hueSlide.Attach(GetDlgItem(IDC_HUE_SLIDER));
	m_hueSlide.SetWindowPos(NULL,730,120,200,30,SWP_SHOWWINDOW);
	m_hueSlide.SetRange(0,255,TRUE);
	m_hueSlide.SetPos(128);
	m_saturationTitle.Attach(GetDlgItem(IDC_STATIC_SATURATION));
	m_saturationTitle.SetWindowText("饱和度");
	m_saturationTitle.SetWindowPos(NULL,730,150,50,20,SWP_SHOWWINDOW);
	m_saturationSlide.Attach(GetDlgItem(IDC_SATURATION_SLIDER));
	m_saturationSlide.SetWindowPos(NULL,730,170,200,30,SWP_SHOWWINDOW);
	m_saturationSlide.SetRange(0,255,TRUE);
	m_saturationSlide.SetPos(128);
	m_brightnessTitle.Attach(GetDlgItem(IDC_STATIC_BRGHTNESS));
	m_brightnessTitle.SetWindowText("亮度");
	m_brightnessTitle.SetWindowPos(NULL,730,200,50,20,SWP_SHOWWINDOW);
	m_brightnessSlide.Attach(GetDlgItem(IDC_BRGHTNESS_SLIDER));
	m_brightnessSlide.SetWindowPos(NULL,730,220,200,30,SWP_SHOWWINDOW);
	m_brightnessSlide.SetRange(0,255,TRUE);
	m_brightnessSlide.SetPos(128);
	m_contrastTitle.Attach(GetDlgItem(IDC_STATIC_CONTRAST));
	m_contrastTitle.SetWindowText("对比度");
	m_contrastTitle.SetWindowPos(NULL,730,250,50,20,SWP_SHOWWINDOW);
	m_contrastSlide.Attach(GetDlgItem(IDC_CONTRAST_SLIDER));
	m_contrastSlide.SetWindowPos(NULL,730,270,200,30,SWP_SHOWWINDOW);
	m_contrastSlide.SetRange(0,255,TRUE);
	m_contrastSlide.SetPos(128);
	m_defaultButton.Attach(GetDlgItem(IDC_BUTTON_DEFAULT));
	m_defaultButton.SetWindowPos(NULL,750,300,50,20,SWP_SHOWWINDOW);

}

void CMainDlg::initTimeLabel()
{
	GetDlgItem(IDC_TIME_LABEL).SetWindowPos(NULL,750,50,200,50,SWP_SHOWWINDOW);
	CString time;
	GetLocalTime(&m_systemTime);
	time.Format("%d.%d.%d   %d:%02d:%02d",m_systemTime.wYear,m_systemTime.wMonth,m_systemTime.wDay,m_systemTime.wHour,m_systemTime.wMinute,m_systemTime.wSecond);
	GetDlgItem(IDC_TIME_LABEL).SetWindowText(time);
}

void CMainDlg::initBottomButton()
{
	GetDlgItem(IDC_BUTTON_OPENPLAY).SetWindowPos(NULL,50,590,100,20,SWP_SHOWWINDOW);
	GetDlgItem(IDC_BUTTON_RECORD_ON).SetWindowPos(NULL,150,590,100,20,SWP_SHOWWINDOW);
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
		MessageBox("大华卡初始化失败!\n");
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
	m_isRecordOn = true;
}



LRESULT CMainDlg::OnBnClickedButtonChoosefile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_playDlg = new CPlayDlg(this);
	m_playDlg->Create(NULL);
	m_playDlg->ShowWindow(TRUE);
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

	m_recordFileName[iChannel].Format("%s%dY%dM%dD%dH%d",m_settingDlg->m_capturePath,systemTime.wYear,systemTime.wMonth,systemTime.wDay,systemTime.wHour,iChannel);
	m_fileHandle[iChannel] = CreateFile(m_recordFileName[iChannel], GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	InsertOneRecord(iChannel,0,systemTime,m_recordFileName[iChannel]);
	if (sub) 
	{
		m_recordFileName[iChannel+BLM_CHANNEL_MAX].Format("%s%dY%dM%dD%dH%dSub",m_settingDlg->m_capturePath,systemTime.wYear,systemTime.wMonth,systemTime.wDay,systemTime.wHour,iChannel);
		m_fileHandle[iChannel+BLM_CHANNEL_MAX] = CreateFile(m_recordFileName[iChannel], GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		InsertOneRecord(iChannel,1,systemTime,m_recordFileName[iChannel]);

		SetupSubChannel(m_channelHandle[iChannel], 1);
		CaptureIFrame(m_channelHandle[iChannel]);
		StartSubVideoCapture(m_channelHandle[iChannel]);
		m_isVideoCapture[iChannel+BLM_CHANNEL_MAX] = TRUE;
	}

	SetupSubChannel(m_channelHandle[iChannel], 0);
	CaptureIFrame(m_channelHandle[iChannel]);
	int iRet = StartVideoCapture(m_channelHandle[iChannel]);

	m_isVideoCapture[iChannel] = TRUE;

}

void CMainDlg::StopCaptureVideo(int iChannel,bool sub)
{
	StopVideoCapture(m_channelHandle[iChannel]);
	if (sub) 
	{
		StopSubVideoCapture(m_channelHandle[iChannel]);
		m_isVideoCapture[iChannel+BLM_CHANNEL_MAX] = FALSE;
	}
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	updateEndTime(systemTime,m_recordFileName[iChannel]);
	CloseHandle(m_fileHandle[iChannel]);
	if(sub)
	{
		updateEndTime(systemTime,m_recordFileName[iChannel+BLM_CHANNEL_MAX]);
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

LRESULT CMainDlg::OnBnClickedButtonRecordOn(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_isRecordOn = !m_isRecordOn;
	if (m_isRecordOn)
	{
		GetDlgItem(IDC_BUTTON_RECORD_ON).SetWindowText("On");
		SetTimer(2,1000);
		SYSTEMTIME st;
		GetLocalTime(&st);
		CheckRecordPlan(st.wDayOfWeek,st.wHour);
	} 
	else
	{
		GetDlgItem(IDC_BUTTON_RECORD_ON).SetWindowText("Off");
		KillTimer(2);
		for (int i =0 ;i<BLM_CHANNEL_MAX;i++)
		{
			if (m_isVideoCapture[i] == TRUE)
			{
				StopCaptureVideo(i,0);
			}
		}
		for (int i =0 ;i<BLM_CHANNEL_MAX;i++)
		{
			if (m_isVideoCapture[i+BLM_CHANNEL_MAX] == TRUE)
			{
				StopCaptureVideo(i,1);
			}
		}
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonDefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_channelHandle[m_focusChannel]!=INVALID_HANDLE_VALUE)
	{
		m_brightnessSlide.SetPos(128);
		m_contrastSlide.SetPos(128);
		m_saturationSlide.SetPos(128);
		m_hueSlide.SetPos(128);
		SetVideoPara(m_channelHandle[m_focusChannel],m_brightnessSlide.GetPos(),m_contrastSlide.GetPos(),m_saturationSlide.GetPos(),m_hueSlide.GetPos());
	}
	return 0;
}

void CMainDlg::initCradleButton()
{
	GetDlgItem(IDC_BUTTON_UP).SetWindowPos(NULL,820,350,30,30,SWP_SHOWWINDOW);
	GetDlgItem(IDC_BUTTON_DOWN).SetWindowPos(NULL,820,430,30,30,SWP_SHOWWINDOW);
	GetDlgItem(IDC_BUTTON_LEFT).SetWindowPos(NULL,780,390,30,30,SWP_SHOWWINDOW);
	GetDlgItem(IDC_BUTTON_RIGHT).SetWindowPos(NULL,860,390,30,30,SWP_SHOWWINDOW);
}

LRESULT CMainDlg::OnBnClickedButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonLeft(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}
