// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "dhvecsystem.h"
#include "PreviewDlg.h"
#include "resource.h"
#include "BlmMessage.h"
#include "SettingDlg.h"
#include "BlmTypes.h"
#include "PlayDlg.h"

enum PreviewLayout{
	PREVIEWLAYOUTFULL = 0,
	PREVIEWLAYOUT1    = 1,
	PREVIEWLAYOUT4    = 4,
	PREVIEWLAYOUT8    = 8
};

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{


private:
	CPlayDlg *m_playDlg;
	HANDLE m_channelHandle[BLM_CHANNEL_MAX];
	CPreiviewDlg  m_previewDlg[BLM_CHANNEL_MAX];
	SYSTEMTIME m_systemTime;
	CTrackBarCtrl m_brightnessSlide,m_contrastSlide,m_saturationSlide,m_hueSlide;
	CStatic m_brightnessTitle,m_contrastTitle,m_saturationTitle,m_hueTitle;
	CButton m_defaultButton;
	CString m_recordFileName[BLM_CHANNEL_MAX*2];
	BOOL m_isFullScreen;
	PreviewLayout m_lastLayout;
	int m_focusChannel;
	void SetPreviewDlgLayout(PreviewLayout layout,int channelID);
	void FocusChannel(int channelID);
	HANDLE m_fileHandle[BLM_CHANNEL_MAX*2];
	BOOL m_isVideoCapture[BLM_CHANNEL_MAX*2];
	BOOL m_isRecordOn;
public:
	CSettingDlg*  m_settingDlg;
	char psText[MAX_PATH*2];
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(BM_CAPTURE_PICTURE, OnCapturePicture)
		MESSAGE_HANDLER(BM_PREVIEW_DBLCLK, OnPreviewDBLCLK)
		MESSAGE_HANDLER(BM_PREVIEW_FOCUS, OnPreviewFocus)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		COMMAND_HANDLER(IDC_LAYOUTBUTTON1, BN_CLICKED, OnBnClickedLayoutbutton1)
		COMMAND_HANDLER(IDC_LAYOUTBUTTON4, BN_CLICKED, OnBnClickedLayoutbutton4)
		COMMAND_HANDLER(IDC_LAYOUTBUTTON8, BN_CLICKED, OnBnClickedLayoutbutton8)
		COMMAND_HANDLER(IDC_SETTINGBUTTON, BN_CLICKED, OnBnClickedSettingbutton)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_HANDLER(IDC_BUTTON_OPENPLAY, BN_CLICKED, OnBnClickedButtonChoosefile)
		COMMAND_HANDLER(IDC_BUTTON_RECORD_ON, BN_CLICKED, OnBnClickedButtonRecordOn)
		COMMAND_HANDLER(IDC_BUTTON_DEFAULT, BN_CLICKED, OnBnClickedButtonDefault)
		COMMAND_HANDLER(IDC_BUTTON_UP, BN_CLICKED, OnBnClickedButtonUp)
		COMMAND_HANDLER(IDC_BUTTON_LEFT, BN_CLICKED, OnBnClickedButtonLeft)
		COMMAND_HANDLER(IDC_BUTTON_DOWN, BN_CLICKED, OnBnClickedButtonDown)
		COMMAND_HANDLER(IDC_BUTTON_RIGHT, BN_CLICKED, OnBnClickedButtonRight)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCapturePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);

	LRESULT OnBnClickedLayoutbutton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedLayoutbutton4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedLayoutbutton8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPreviewDBLCLK(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPreviewFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedSettingbutton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void updateSetting();
	void initSlide();
	void initTimeLabel();
	void initBottomButton();
	void initDH();
	void initPreviewDlg();
	void initValue();
	LRESULT OnBnClickedButtonChoosefile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void SaveStreamData(int iChannel, void * pData, int iNum, int iFrameType);
	void StartCaptureVideo(int iChannel,bool sub);
	void StopCaptureVideo(int iChannel,bool sub);
	void CheckRecordPlan(int dayofWeek,int hour);
	LRESULT OnBnClickedButtonRecordOn(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonDefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void initCradleButton();
	LRESULT OnBnClickedButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonLeft(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
